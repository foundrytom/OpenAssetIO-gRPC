// SPDX-License-Identifier: Apache-2.0
// Copyright 2013-2023 The Foundry Visionmongers Ltd
#include <iostream>

#include <grpcpp/grpcpp.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include "openassetio.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class ManagerProxyImpl final : public openassetio_grpc_proto::ManagerProxy::Service {
 public:
  Status Identifiers([[maybe_unused]] ServerContext* context,
                     [[maybe_unused]] const openassetio_grpc_proto::EmptyRequest* request,
                     ::openassetio_grpc_proto::IdentifiersResponse* response) override {
    response->add_identifer("com.manager.a");
    response->add_identifer("com.manager.b");
    return Status::OK;
  };
};

void runServer() {
  std::string serverAddress("0.0.0.0:50051");
  ManagerProxyImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << serverAddress << std::endl;
  server->Wait();
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
  runServer();
  return 0;
}
