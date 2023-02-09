// SPDX-License-Identifier: Apache-2.0
// Copyright 2013-2023 The Foundry Visionmongers Ltd
#include "openassetio-grpc/GRPCManagerInterface.hpp"

#include <grpcpp/grpcpp.h>

#include "openassetio.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

namespace openassetio::grpc {

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


  void destroy() {
    openassetio_grpc_proto::DestroyRequest request;
    openassetio_grpc_proto::EmptyResponse response;
    ClientContext context;

    request.set_handle(handle_);

    stub_->Destroy(&context, request, &response);
  }

  const std::string &handle() const { return handle_; }

 private:
  std::unique_ptr<openassetio_grpc_proto::ManagerProxy::Stub> stub_;
  const GRPCManagerInterface::RemoteHandle handle_;
};

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
  return trait::TraitsDatas{};
}

bool GRPCManagerInterface::isEntityReferenceString(
    [[maybe_unused]] const Str &someString,
    [[maybe_unused]] const managerApi::HostSessionPtr &hostSession) const {
  return false;
}

void GRPCManagerInterface::initialize(
    [[maybe_unused]] InfoDictionary managerSettings,
    [[maybe_unused]] const managerApi::HostSessionPtr &hostSession) {}

void GRPCManagerInterface::resolve(
    [[maybe_unused]] const EntityReferences &entityReferences,
    [[maybe_unused]] const trait::TraitSet &traitSet,
    [[maybe_unused]] const ContextConstPtr &context,
    [[maybe_unused]] const managerApi::HostSessionPtr &hostSession,
    [[maybe_unused]] const ResolveSuccessCallback &successCallback,
    [[maybe_unused]] const BatchElementErrorCallback &errorCallback) {}

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
