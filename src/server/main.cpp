// SPDX-License-Identifier: Apache-2.0
// Copyright 2013-2023 The Foundry Visionmongers Ltd
#include <iostream>
#include <sstream>
#include <map>

#include <Python.h>

#include <grpcpp/grpcpp.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include <openassetio/hostApi/ManagerImplementationFactoryInterface.hpp>
#include <openassetio/log/ConsoleLogger.hpp>
#include <openassetio/log/SeverityFilter.hpp>
#include <openassetio/managerApi/ManagerInterface.hpp>
#include <openassetio/python/hostApi.hpp>

#include "openassetio.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using openassetio::log::ConsoleLogger;
using openassetio::log::SeverityFilter;
using openassetio::managerApi::ManagerInterfacePtr;


class ManagerProxyImpl final : public openassetio_grpc_proto::ManagerProxy::Service {
 public:
  explicit ManagerProxyImpl(openassetio::log::LoggerInterfacePtr logger)
      : logger_(std::move(logger)) {
    implementationFactory_ =
        openassetio::python::hostApi::createPythonPluginSystemManagerImplementationFactory(
            logger_);
  }

  Status Identifiers([[maybe_unused]] ServerContext* context,
                     [[maybe_unused]] const openassetio_grpc_proto::EmptyRequest* request,
                     ::openassetio_grpc_proto::IdentifiersResponse* response) override {
    for (const openassetio::Identifier& identifier : implementationFactory_->identifiers()) {
      response->add_identifer(identifier);
    }
    return Status::OK;
  };

  Status Instantiate([[maybe_unused]] ServerContext* context,
                     const openassetio_grpc_proto::InstantiateRequest* request,
                     ::openassetio_grpc_proto::InstantiateResponse* response) override {
    const openassetio::Identifier& identifier = request->identifier();
    // TODO(tc) Handle exceptions here
    const ManagerInterfacePtr managerInterface =
        implementationFactory_->instantiate(identifier);
    std::stringstream handle;
    handle << static_cast<void const *>(managerInterface.get());
    managers_.insert({handle.str(), managerInterface});
    response->set_remotehandle(handle.str());
    logger_->debugApi("Instantiated " + identifier + " with handle " + handle.str());
    return Status::OK;
  }

  Status Destroy([[maybe_unused]] ServerContext* context,
                 const openassetio_grpc_proto::DestroyRequest* request,
                 [[maybe_unused]] ::openassetio_grpc_proto::EmptyResponse * response) override {

    const auto& iter = managers_.find(request->remotehandle());
    if (iter == managers_.end()) {
      // TODO(tc): Error handling
      logger_->warning("Requested to destroy non-existent handle " + request->remotehandle());
      return Status::OK;
    }
    managers_.erase(iter);
    logger_->debugApi("Destoryed " + request->remotehandle());
    return Status::OK;
}


 private:
  openassetio::log::LoggerInterfacePtr logger_;
  openassetio::hostApi::ManagerImplementationFactoryInterfacePtr implementationFactory_;
  std::map<std::string, ManagerInterfacePtr> managers_;
};

void runServer() {
  Py_Initialize();
  {
    Py_BEGIN_ALLOW_THREADS auto logger = SeverityFilter::make(ConsoleLogger::make());
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
