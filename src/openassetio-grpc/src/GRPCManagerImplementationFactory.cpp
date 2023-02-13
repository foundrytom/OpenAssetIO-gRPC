// SPDX-License-Identifier: Apache-2.0
// Copyright 2013-2023 The Foundry Visionmongers Ltd
#include "openassetio-grpc/GRPCManagerImplementationFactory.hpp"

#include <iostream>
#include <stdexcept>
#include <utility>

#include <grpcpp/grpcpp.h>

#include <openassetio/hostApi/ManagerImplementationFactoryInterface.hpp>
#include <openassetio/log/LoggerInterface.hpp>

#include "openassetio.grpc.pb.h"

#include "openassetio-grpc/GRPCManagerInterface.hpp"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

namespace openassetio::grpc {

class GRPCManagerImplementationFactoryClient {
 public:
  explicit GRPCManagerImplementationFactoryClient(std::shared_ptr<Channel> channel)
      : stub_(openassetio_grpc_proto::ManagerProxy::NewStub(std::move(channel))) {}

  std::vector<std::string> identifiers() {
    openassetio_grpc_proto::EmptyRequest request;
    openassetio_grpc_proto::IdentifiersResponse response;
    ClientContext context;

    Status status = stub_->Identifiers(&context, request, &response);
    if (!status.ok()) {
      throw std::runtime_error(status.error_message());
    }

    const int count = response.identifer_size();
    std::vector<std::string> identifiers;
    identifiers.reserve(size_t(count));
    for (int i = 0; i < count; ++i) {
      identifiers.push_back(response.identifer(i));
    }
    return identifiers;
  }

  GRPCManagerInterface::RemoteHandle instantiate(const openassetio::Identifier& identifier) {
    openassetio_grpc_proto::InstantiateRequest request;
    openassetio_grpc_proto::InstantiateResponse response;
    ClientContext context;

    request.set_identifier(identifier);

    Status status = stub_->Instantiate(&context, request, &response);
    if (!status.ok()) {
      throw std::runtime_error(status.error_message());
    }

    return GRPCManagerInterface::RemoteHandle(response.handle());
  }

 private:
  std::unique_ptr<openassetio_grpc_proto::ManagerProxy::Stub> stub_;
};

GRPCManagerImplementationFactoryPtr GRPCManagerImplementationFactory::make(
    const std::string& channel, log::LoggerInterfacePtr logger) {
  return std::shared_ptr<GRPCManagerImplementationFactory>(
      new GRPCManagerImplementationFactory(channel, std::move(logger)));
}

GRPCManagerImplementationFactory::GRPCManagerImplementationFactory(const std::string& channel,
                                                                   log::LoggerInterfacePtr logger)
    : ManagerImplementationFactoryInterface(std::move(logger)),
      client_(new GRPCManagerImplementationFactoryClient(
          ::grpc::CreateChannel(channel, ::grpc::InsecureChannelCredentials()))),
      channel_(channel) {}

GRPCManagerImplementationFactory::~GRPCManagerImplementationFactory() = default;

Identifiers GRPCManagerImplementationFactory::identifiers() { return client_->identifiers(); }

managerApi::ManagerInterfacePtr GRPCManagerImplementationFactory::instantiate(
    [[maybe_unused]] const Identifier& identifier) {
  GRPCManagerInterface::RemoteHandle handle = client_->instantiate(identifier);
  logger_->debugApi("gRPC: Instantiated '" + identifier + "' [" + handle + "]");
  return std::make_shared<GRPCManagerInterface>(handle, channel_);
}

}  // namespace openassetio::grpc
