// SPDX-License-Identifier: Apache-2.0
// Copyright 2013-2023 The Foundry Visionmongers Ltd
#include <iostream>

#include <Python.h>

#include <grpcpp/grpcpp.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include <openassetio/hostApi/HostInterface.hpp>
#include <openassetio/hostApi/ManagerFactory.hpp>
#include <openassetio/log/ConsoleLogger.hpp>
#include <openassetio/log/SeverityFilter.hpp>
#include <openassetio/python/hostApi.hpp>

#include "openassetio.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using openassetio::hostApi::ManagerFactory;
using openassetio::hostApi::ManagerFactoryPtr;
using openassetio::log::ConsoleLogger;
using openassetio::log::SeverityFilter;

class TestHostInterface : public openassetio::hostApi::HostInterface {
  [[nodiscard]] openassetio::Identifier identifier() const override {
    return "org.openassetio.gRPC.testHost";
  }

  [[nodiscard]] openassetio::Str displayName() const override {
    return "OpenAssetIO gRPC Test Host";
  }
};

class ManagerProxyImpl final : public openassetio_grpc_proto::ManagerProxy::Service {
 public:
  explicit ManagerProxyImpl(openassetio::log::LoggerInterfacePtr logger) : logger_(std::move(logger)) {
    logger_->debug("Creating TestHostInterface");
    const auto hostInterface = std::make_shared<TestHostInterface>();
    logger_->debug("Creating PythonPluginSystemManagerImplementationFactory");
    const auto pythonImplementationFactory =
        openassetio::python::hostApi::createPythonPluginSystemManagerImplementationFactory(
            logger_);
    logger_->debug("Creating ManagerFactory");
    managerFactory_ = ManagerFactory::make(hostInterface, pythonImplementationFactory, logger_);
  }

  Status Identifiers([[maybe_unused]] ServerContext* context,
                     [[maybe_unused]] const openassetio_grpc_proto::EmptyRequest* request,
                     ::openassetio_grpc_proto::IdentifiersResponse* response) override {
    logger_->debug("Idenfifiers called");
    for (const openassetio::Identifier& identifier : managerFactory_->identifiers()) {
      response->add_identifer(identifier);
      logger_->debug(identifier);
    }
    logger_->debug("OK");
    return Status::OK;
  };

 private:
  openassetio::log::LoggerInterfacePtr logger_;
  ManagerFactoryPtr managerFactory_;
};

void runServer() {
  Py_Initialize();
  {
    Py_BEGIN_ALLOW_THREADS
    auto logger = SeverityFilter::make(ConsoleLogger::make());
    std::string serverAddress("0.0.0.0:50051");
    ManagerProxyImpl service{logger};

    ServerBuilder builder;
    builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());

    logger->info("Server listening on " + serverAddress);
    server->Wait();
    Py_END_ALLOW_THREADS
  }
  Py_FinalizeEx();
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
  runServer();
  return 0;
}
