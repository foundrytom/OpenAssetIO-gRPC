// SPDX-License-Identifier: Apache-2.0
// Copyright 2013-2023 The Foundry Visionmongers Ltd
#include "openassetio-grpc/GRPCManagerImplementationFactory.hpp"

#include <iostream>

#include <grpcpp/grpcpp.h>

#include "openassetio.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using openassetio_grpc_proto::EmptyRequest;
using openassetio_grpc_proto::IdentifiersResponse;
using openassetio_grpc_proto::ManagerProxy;

namespace openassetio::grpc {

class GRPCManagerImplementationFactoryClient {
 public:
  explicit GRPCManagerImplementationFactoryClient(std::shared_ptr<Channel> channel)
      : stub_(ManagerProxy::NewStub(channel)) {}

  std::vector<std::string> identifiers() {
    EmptyRequest request;
    IdentifiersResponse response;

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

 private:
  std::unique_ptr<ManagerProxy::Stub> stub_;
};

GRPCManagerImplementationFactory::GRPCManagerImplementationFactory(const std::string& channel)
    : client_(new GRPCManagerImplementationFactoryClient(
          ::grpc::CreateChannel(channel, ::grpc::InsecureChannelCredentials()))) {}

GRPCManagerImplementationFactory::~GRPCManagerImplementationFactory() = default;

std::vector<std::string> GRPCManagerImplementationFactory::identifiers() {
  return client_->identifiers();
}

}  // namespace openassetio::grpc
