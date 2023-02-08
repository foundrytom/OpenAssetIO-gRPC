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
  explicit GRPCManagerInterfaceClient(std::shared_ptr<Channel> channel)
      : stub_(openassetio_grpc_proto::ManagerProxy::NewStub(std::move(channel))) {}

  void destroy(const GRPCManagerInterface::RemoteHandle &handle) {
    openassetio_grpc_proto::DestroyRequest request;
    openassetio_grpc_proto::EmptyResponse response;
    ClientContext context;

    request.set_remotehandle(handle);

    Status status = stub_->Destroy(&context, request, &response);
  }

 private:
  std::unique_ptr<openassetio_grpc_proto::ManagerProxy::Stub> stub_;
};

GRPCManagerInterface::GRPCManagerInterface(GRPCManagerInterface::RemoteHandle handle,
                                           const std::string &channel)
    : handle_(std::move(handle)),
      client_(new GRPCManagerInterfaceClient(
          ::grpc::CreateChannel(channel, ::grpc::InsecureChannelCredentials()))) {}

GRPCManagerInterface::~GRPCManagerInterface() {
  try {
    client_->destroy(handle_);
  } catch (...) {
  }
}

Str GRPCManagerInterface::displayName() const {
  return "OpenAssetIO gRPC Proxy [" + handle_ + "]";
}

Identifier GRPCManagerInterface::identifier() const { return "org.openassetio.manager.gRPC"; }

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
