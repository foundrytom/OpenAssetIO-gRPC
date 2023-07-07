// SPDX-License-Identifier: Apache-2.0
// Copyright 2013-2023 The Foundry Visionmongers Ltd
#include <iostream>
#include <map>
#include <openassetio/BatchElementError.hpp>
#include <openassetio/trait/collection.hpp>
#include <sstream>

#include <Python.h>

#include <ceval.h>
#include <grpc/status.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include <openassetio/Context.hpp>
#include <openassetio/EntityReference.hpp>
#include <openassetio/hostApi/ManagerImplementationFactoryInterface.hpp>
#include <openassetio/log/ConsoleLogger.hpp>
#include <openassetio/log/SeverityFilter.hpp>
#include <openassetio/managerApi/ManagerInterface.hpp>
#include <openassetio/python/hostApi.hpp>

#include "openassetio.grpc.pb.h"
#include "openassetio.pb.h"
#include "utils.hpp"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using openassetio::ContextPtr;
using openassetio::EntityReference;
using openassetio::log::ConsoleLogger;
using openassetio::log::SeverityFilter;
using openassetio::managerApi::HostSessionPtr;
using openassetio::managerApi::ManagerInterfacePtr;

/**
 * The server is a very simplistic, hacky gRPC unary gRPC server based
 * off the quickstart guides.
 *
 * It ignores the normal Manager/ManagerFactory layer of OpenAssetIO and
 * works directly at the lower level by providing external access to a
 * ManagerImplementationFactoryInterface, and then the corresponding
 * methods of any instantiated ManagerInterfaces.
 *
 * This greatly simplifies proxying methods to a Host, as its tasks are
 * then two fold:
 *
 *  - Instantiate ManagerInterfaces on demand, assign them a handle, and
 *    keep them alive until otherwise requested.
 *
 *  - Convert incoming gRPC messages to their equivalent OpenAssetIO
 *    data types, invoke the methods on their corresponding
 *    ManagerInterface instance, convert the result to gPRC messages for
 *    the response.
 *
 * At present, instantiated managers are kept in a map, using their memory
 * address as a unique key and as the handle used in gRPC messages.
 *
 * @todo This needs a lot of tidying and work on error handling for
 * anything other than the happy path.
 */

class ManagerProxyImpl final : public openassetio_grpc_proto::ManagerProxy::Service {
 public:
  explicit ManagerProxyImpl(openassetio::log::LoggerInterfacePtr logger)
      : logger_(std::move(logger)) {
    implementationFactory_ =
        openassetio::python::hostApi::createPythonPluginSystemManagerImplementationFactory(
            logger_);
  }

  // Factory methods

  Status Identifiers([[maybe_unused]] ServerContext* context,
                     [[maybe_unused]] const openassetio_grpc_proto::EmptyRequest* request,
                     ::openassetio_grpc_proto::IdentifiersResponse* response) override {
    for (const openassetio::Identifier& identifier : implementationFactory_->identifiers()) {
      response->add_identifier(identifier);
    }
    return Status::OK;
  };

  Status Instantiate([[maybe_unused]] ServerContext* context,
                     const openassetio_grpc_proto::InstantiateRequest* request,
                     ::openassetio_grpc_proto::InstantiateResponse* response) override {
    const openassetio::Identifier& identifier = request->identifier();
    // TODO(tc) Handle exceptions here
    const ManagerInterfacePtr managerInterface = implementationFactory_->instantiate(identifier);
    std::stringstream handle;
    handle << static_cast<void const*>(managerInterface.get());
    managers_.insert({handle.str(), managerInterface});
    response->set_handle(handle.str());
    logger_->debugApi("Instantiated " + identifier + " with handle " + handle.str());
    return Status::OK;
  }

  Status Destroy([[maybe_unused]] ServerContext* context,
                 const openassetio_grpc_proto::DestroyRequest* request,
                 [[maybe_unused]] ::openassetio_grpc_proto::EmptyResponse* response) override {
    const auto& iter = managers_.find(request->handle());
    if (iter == managers_.end()) {
      // TODO(tc): Error handling
      logger_->warning("Requested to destroy non-existent handle " + request->handle());
      return Status::OK;
    }
    managers_.erase(iter);
    logger_->debugApi("Destroyed " + request->handle());
    return Status::OK;
  }

  // ManagerInterface methods

  Status Identifier([[maybe_unused]] ServerContext* context,
                    const openassetio_grpc_proto::IdentifierRequest* request,
                    ::openassetio_grpc_proto::IdentifierResponse* response) override {
    if (ManagerInterfacePtr manager = managerFromHandle(request->handle())) {
      logger_->debugApi(request->handle() + " identifier()");
      response->set_identifier(manager->identifier());
      return Status::OK;
    }
    logger_->error("Identifier: Unknown handle " + request->handle());
    return Status::CANCELLED;
  }

  Status DisplayName([[maybe_unused]] ServerContext* context,
                     const openassetio_grpc_proto::DisplayNameRequest* request,
                     ::openassetio_grpc_proto::DisplayNameResponse* response) override {
    if (ManagerInterfacePtr manager = managerFromHandle(request->handle())) {
      logger_->debugApi(request->handle() + " displayName()");
      response->set_displayname(manager->displayName());
      return Status::OK;
    }
    logger_->error("DisplayName: Unknown handle " + request->handle());
    return Status::CANCELLED;
  }

  Status Info([[maybe_unused]] ServerContext* context,
                     const openassetio_grpc_proto::InfoRequest* request,
                     ::openassetio_grpc_proto::InfoResponse* response) override {
    if (ManagerInterfacePtr manager = managerFromHandle(request->handle())) {
      logger_->debugApi(request->handle() + " info()");
      openassetio::grpc::infoDictionaryToMsg(manager->info(), response->mutable_info());
      return Status::OK;
    }
    logger_->error("DisplayName: Unknown handle " + request->handle());
    return Status::CANCELLED;
  }

  Status Initialize([[maybe_unused]] ServerContext* context,
                    const openassetio_grpc_proto::InitializeRequest* request,
                    [[maybe_unused]] ::openassetio_grpc_proto::EmptyResponse* response) override {
    if (ManagerInterfacePtr manager = managerFromHandle(request->handle())) {
      try {
        HostSessionPtr hostSession =
            openassetio::grpc::msgToHostSession(request->hostsession(), logger_);
        openassetio::InfoDictionary managerSettings =
            openassetio::grpc::msgToInfoDictionary(request->settings());

        logger_->debugApi(request->handle() + " initialize()");
        manager->initialize(managerSettings, hostSession);

        return Status::OK;
      } catch (std::exception& e) {
        const std::string msg{e.what()};
        return Status{grpc::StatusCode::ABORTED, msg};
      }
    }
    logger_->error("Initialize: Unknown handle " + request->handle());
    return Status::CANCELLED;
  }

  Status ManagementPolicy([[maybe_unused]] ServerContext* serverContext,
                          const openassetio_grpc_proto::ManagementPolicyRequest* request,
                          openassetio_grpc_proto::ManagementPolicyResponse* response) override {
    if (ManagerInterfacePtr manager = managerFromHandle(request->handle())) {
      try {
        HostSessionPtr hostSession =
            openassetio::grpc::msgToHostSession(request->hostsession(), logger_);
        ContextPtr context = openassetio::grpc::msgToContext(request->context());

        openassetio::trait::TraitSets sets;
        for (const auto& setMsg : request->traitset()) {
          sets.push_back(openassetio::grpc::msgToTraitSet(setMsg));
        }

        logger_->debugApi(request->handle() + " managementPolicy()");
        auto policies = manager->managementPolicy(sets, context, hostSession);

        for (const auto& traitsData : policies) {
          auto* policyMsg = response->add_policy();
          openassetio::grpc::traitsDataToMsg(traitsData, policyMsg);
        }

        return Status::OK;
      } catch (std::exception& e) {
        const std::string msg{e.what()};
        return Status{grpc::StatusCode::ABORTED, msg};
      }
    }
    logger_->error("Initialize: Unknown handle " + request->handle());
    return Status::CANCELLED;
  }

  Status IsEntityReferenceString(
      [[maybe_unused]] ServerContext* context,
      const openassetio_grpc_proto::IsEntityReferenceStringRequest* request,
      openassetio_grpc_proto::IsEntityReferenceStringResponse* response) override {
    if (ManagerInterfacePtr manager = managerFromHandle(request->handle())) {
      try {
        HostSessionPtr hostSession =
            openassetio::grpc::msgToHostSession(request->hostsession(), logger_);

        logger_->debugApi(request->handle() + " isEntityReferenceString()");
        response->set_is(manager->isEntityReferenceString(request->somestring(), hostSession));

        return Status::OK;
      } catch (std::exception& e) {
        const std::string msg{e.what()};
        return Status{grpc::StatusCode::ABORTED, msg};
      }
    }
    logger_->error("Initialize: Unknown handle " + request->handle());
    return Status::CANCELLED;
  }

  Status Resolve([[maybe_unused]] ServerContext* serverContext,
                 const openassetio_grpc_proto::ResolveRequest* request,
                 openassetio_grpc_proto::ResolveResponse* response) override {
    if (ManagerInterfacePtr manager = managerFromHandle(request->handle())) {
      try {
        HostSessionPtr hostSession =
            openassetio::grpc::msgToHostSession(request->hostsession(), logger_);
        ContextPtr context = openassetio::grpc::msgToContext(request->context());

        openassetio::EntityReferences refs;
        for (const auto& refStr : request->entityreference()) {
          // TODO(tc) check first with isEntityReferenceString?
          refs.push_back(EntityReference{refStr});
        }

        logger_->debugApi(request->handle() + " resolve()");
        manager->resolve(
            refs, openassetio::grpc::msgToTraitSet(request->traitset()), context, hostSession,
            [&response](std::size_t index, const openassetio::TraitsDataPtr& data) {
              auto* elementMsg = response->add_resultorerror();
              elementMsg->set_index(index);
              openassetio::grpc::traitsDataToMsg(data, elementMsg->mutable_result());
            },
            [&response](std::size_t index, const openassetio::BatchElementError& error) {
              auto* elementMsg = response->add_resultorerror();
              elementMsg->set_index(index);
              auto* errorMsg = elementMsg->mutable_error();
              errorMsg->set_code(int32_t(error.code));
              errorMsg->set_errormessage(error.message);
            });

        return Status::OK;
      } catch (std::exception& e) {
        const std::string msg{e.what()};
        return Status{grpc::StatusCode::ABORTED, msg};
      }
    }
    logger_->error("Initialize: Unknown handle " + request->handle());
    return Status::CANCELLED;
  }

 private:
  [[nodiscard]] ManagerInterfacePtr managerFromHandle(const std::string& handle) const {
    const auto& iter = managers_.find(handle);
    if (iter == managers_.end()) {
      return nullptr;
    }
    return iter->second;
  }

  openassetio::log::LoggerInterfacePtr logger_;
  openassetio::hostApi::ManagerImplementationFactoryInterfacePtr implementationFactory_;
  // A simple map of handles to instances that keeps manager
  // implementations alive while needed.
  // @todo Thread safety
  std::map<std::string, ManagerInterfacePtr> managers_;
};

void runServer() {
  Py_Initialize();
  // Scope ensures that all OpenAssetIO Py objects are deleted before
  // Py_FinalizeEx.
  {
    // We need to release the GIL so that the gRPC worker threads can
    // call into Python as needed.
    Py_BEGIN_ALLOW_THREADS

      // @todo Allow this to be configured
    std::string serverAddress("0.0.0.0:50051");

    auto logger = SeverityFilter::make(ConsoleLogger::make());

    ManagerProxyImpl service{logger};

    // @todo Error handling
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
