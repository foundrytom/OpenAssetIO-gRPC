// SPDX-License-Identifier: Apache-2.0
// Copyright 2013-2023 The Foundry Visionmongers Ltd
#include "openassetio-grpc/GRPCManagerInterface.hpp"

#include <grpcpp/grpcpp.h>
#include <openassetio/BatchElementError.hpp>
#include <openassetio/managerApi/ManagerInterface.hpp>
#include <openassetio/trait/collection.hpp>

#include "openassetio.grpc.pb.h"
#include "openassetio.pb.h"
#include "utils.hpp"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

/**
 * The gRPC code is split out into its own class to help contain it as
 * an implementation detail.
 */

namespace openassetio::grpc {

/**
 * gRPC client
 *
 * The client simply bridges the OpenAssetIO method signatures into gPRC
 * messages, makes the request, and unpacks the result.
 *
 * Warning: Noddy/noob gRPC programming ahead!
 */

class GRPCManagerInterfaceClient {
 public:
  explicit GRPCManagerInterfaceClient(std::shared_ptr<Channel> channel,
                                      GRPCManagerInterface::RemoteHandle handle)
      : stub_(openassetio_grpc_proto::ManagerProxy::NewStub(std::move(channel))),
        handle_(std::move(handle)) {}

  std::string identifier() {
    openassetio_grpc_proto::IdentifierRequest request;
    openassetio_grpc_proto::IdentifierResponse response;
    ClientContext context;

    request.set_handle(handle_);

    Status status = stub_->Identifier(&context, request, &response);
    if (!status.ok()) {
      throw std::runtime_error(status.error_message());
    }
    return response.identifier();
  }

  std::string displayName() {
    openassetio_grpc_proto::DisplayNameRequest request;
    openassetio_grpc_proto::DisplayNameResponse response;
    ClientContext context;

    request.set_handle(handle_);

    Status status = stub_->DisplayName(&context, request, &response);
    if (!status.ok()) {
      throw std::runtime_error(status.error_message());
    }
    return response.displayname();
  }

  void initialize(const InfoDictionary &managerSettings,
                  const managerApi::HostSessionPtr &hostSession) {
    openassetio_grpc_proto::InitializeRequest request;
    openassetio_grpc_proto::EmptyResponse response;
    ClientContext context;

    request.set_handle(handle_);
    hostSessionToMsg(hostSession, request.mutable_hostsession());
    infoDictionaryToMsg(managerSettings, request.mutable_settings());

    Status status = stub_->Initialize(&context, request, &response);
    if (!status.ok()) {
      throw std::runtime_error(status.error_message());
    }
  }

  trait::TraitsDatas managementPolicy(const trait::TraitSets &traitSets,
                                      const ContextConstPtr &context,
                                      const managerApi::HostSessionPtr &hostSession) const {
    openassetio_grpc_proto::ManagementPolicyRequest request;
    openassetio_grpc_proto::ManagementPolicyResponse response;
    ClientContext clientContext;

    request.set_handle(handle_);
    contextToMsg(context, request.mutable_context());
    hostSessionToMsg(hostSession, request.mutable_hostsession());

    for (const trait::TraitSet &set : traitSets) {
      auto *msg = request.add_traitset();
      traitSetToMsg(set, msg);
    }

    Status status = stub_->ManagementPolicy(&clientContext, request, &response);
    if (!status.ok()) {
      throw std::runtime_error(status.error_message());
    }

    trait::TraitsDatas datas;
    for (const auto &dataMsg : response.policy()) {
      datas.push_back(msgToTraitsData(dataMsg));
    }
    return datas;
  }

  bool isEntityReferenceString(const Str &someString,
                               const managerApi::HostSessionPtr &hostSession) const {
    openassetio_grpc_proto::IsEntityReferenceStringRequest request;
    openassetio_grpc_proto::IsEntityReferenceStringResponse response;
    ClientContext context;

    request.set_handle(handle_);
    request.set_somestring(someString);
    hostSessionToMsg(hostSession, request.mutable_hostsession());

    Status status = stub_->IsEntityReferenceString(&context, request, &response);
    if (!status.ok()) {
      throw std::runtime_error(status.error_message());
    }

    return response.is();
  }

  // Management

  void destroy() {
    openassetio_grpc_proto::DestroyRequest request;
    openassetio_grpc_proto::EmptyResponse response;
    ClientContext context;

    request.set_handle(handle_);

    stub_->Destroy(&context, request, &response);
  }

  void resolve(const EntityReferences &entityReferences, const trait::TraitSet &traitSet,
               const ContextConstPtr &context, const managerApi::HostSessionPtr &hostSession,
               const GRPCManagerInterface::ResolveSuccessCallback &successCallback,
               const BatchElementErrorCallback &errorCallback) {
    openassetio_grpc_proto::ResolveRequest request;
    openassetio_grpc_proto::ResolveResponse response;
    ClientContext clientContext;

    request.set_handle(handle_);
    hostSessionToMsg(hostSession, request.mutable_hostsession());
    contextToMsg(context, request.mutable_context());
    traitSetToMsg(traitSet, request.mutable_traitset());

    for (const EntityReference &ref : entityReferences) {
      *(request.add_entityreference()) = ref.toString();
    }

    Status status = stub_->Resolve(&clientContext, request, &response);
    if (!status.ok()) {
      throw std::runtime_error(status.error_message());
    }

    for (const auto &resultOrError : response.resultorerror()) {
      if (resultOrError.has_result()) {
        successCallback(resultOrError.index(), msgToTraitsData(resultOrError.result()));
      } else {
        const auto &error = resultOrError.error();
        errorCallback(
            resultOrError.index(),
            BatchElementError{BatchElementError::ErrorCode(error.code()), error.errormessage()});
      }
    }
  }

  const std::string &handle() const { return handle_; }

 private:
  std::unique_ptr<openassetio_grpc_proto::ManagerProxy::Stub> stub_;
  const GRPCManagerInterface::RemoteHandle handle_;
};

/**
 * ManagerInterface implementation
 *
 * This is just a thin wrapper around the client.
 *
 * @todo Proper error handling, right now gRPC failures (including
 * server-side exceptions) are simply re-thrown as runtime_errors.
 *
 * @todo Remaining (incl optional) interface methods. We could also
 * use the handle based mechanism to manage the managerInterfaceState
 * objects.
 */

GRPCManagerInterface::GRPCManagerInterface(GRPCManagerInterface::RemoteHandle handle,
                                           const std::string &channel)
    : client_(new GRPCManagerInterfaceClient(
          ::grpc::CreateChannel(channel, ::grpc::InsecureChannelCredentials()),
          std::move(handle))) {}

GRPCManagerInterface::~GRPCManagerInterface() {
  try {
    client_->destroy();
  } catch (...) {
  }
}

Identifier GRPCManagerInterface::identifier() const { return client_->identifier(); }

Str GRPCManagerInterface::displayName() const { return client_->displayName(); }

trait::TraitsDatas GRPCManagerInterface::managementPolicy(
    [[maybe_unused]] const trait::TraitSets &traitSets,
    [[maybe_unused]] const ContextConstPtr &context,
    [[maybe_unused]] const managerApi::HostSessionPtr &hostSession) const {
  return client_->managementPolicy(traitSets, context, hostSession);
}

bool GRPCManagerInterface::isEntityReferenceString(
    [[maybe_unused]] const Str &someString,
    [[maybe_unused]] const managerApi::HostSessionPtr &hostSession) const {
  return client_->isEntityReferenceString(someString, hostSession);
}

void GRPCManagerInterface::initialize(
    [[maybe_unused]] InfoDictionary managerSettings,
    [[maybe_unused]] const managerApi::HostSessionPtr &hostSession) {
  client_->initialize(managerSettings, hostSession);
}

void GRPCManagerInterface::resolve(
    [[maybe_unused]] const EntityReferences &entityReferences,
    [[maybe_unused]] const trait::TraitSet &traitSet,
    [[maybe_unused]] const ContextConstPtr &context,
    [[maybe_unused]] const managerApi::HostSessionPtr &hostSession,
    [[maybe_unused]] const ResolveSuccessCallback &successCallback,
    [[maybe_unused]] const BatchElementErrorCallback &errorCallback) {
  client_->resolve(entityReferences, traitSet, context, hostSession, successCallback,
                   errorCallback);
}

void GRPCManagerInterface::preflight(
    [[maybe_unused]] const EntityReferences &entityReferences,
    [[maybe_unused]] const trait::TraitSet &traitSet,
    [[maybe_unused]] const ContextConstPtr &context,
    [[maybe_unused]] const managerApi::HostSessionPtr &hostSession,
    [[maybe_unused]] const PreflightSuccessCallback &successCallback,
    [[maybe_unused]] const BatchElementErrorCallback &errorCallback) {}

void GRPCManagerInterface::register_(
    [[maybe_unused]] const EntityReferences &entityReferences,
    [[maybe_unused]] const trait::TraitsDatas &entityTraitsDatas,
    [[maybe_unused]] const ContextConstPtr &context,
    [[maybe_unused]] const managerApi::HostSessionPtr &hostSession,
    [[maybe_unused]] const RegisterSuccessCallback &successCallback,
    [[maybe_unused]] const BatchElementErrorCallback &errorCallback) {}

};  // namespace openassetio::grpc
