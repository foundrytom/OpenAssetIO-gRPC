// SPDX-License-Identifier: Apache-2.0
// Copyright 2013-2023 The Foundry Visionmongers Ltd
#include <iostream>
#include <memory>

#include <openassetio/Context.hpp>
#include <openassetio/EntityReference.hpp>
#include <openassetio/TraitsData.hpp>
#include <openassetio/hostApi/HostInterface.hpp>
#include <openassetio/hostApi/Manager.hpp>
#include <openassetio/hostApi/ManagerFactory.hpp>
#include <openassetio/log/ConsoleLogger.hpp>
#include <openassetio/log/SeverityFilter.hpp>
#include <openassetio/trait/property.hpp>

#include <openassetio-grpc/GRPCManagerImplementationFactory.hpp>

using openassetio::Context;
using openassetio::ContextPtr;
using openassetio::EntityReference;
using openassetio::hostApi::ManagerFactory;
using openassetio::hostApi::ManagerFactoryPtr;
using openassetio::hostApi::ManagerPtr;
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

using openassetio::grpc::GRPCManagerImplementationFactory;
using openassetio::grpc::GRPCManagerImplementationFactoryPtr;

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
  openassetio::log::LoggerInterfacePtr logger = SeverityFilter::make(ConsoleLogger::make());

  GRPCManagerImplementationFactoryPtr implFactory =
      std::make_shared<GRPCManagerImplementationFactory>("0.0.0.0:50051", logger);

  openassetio::hostApi::HostInterfacePtr hostInterface = std::make_shared<TestHostInterface>();

  ManagerFactoryPtr factory = ManagerFactory::make(hostInterface, implFactory, logger);
  logger->info("Available managers:");
  for (auto& [identifier, detail] : factory->availableManagers()) {
    logger->info(detail.displayName + " [" + detail.identifier + "]");
  }
  logger->info("Done");

  // Initialize the default manager
  logger->info("Default manager:");
  ManagerPtr defaultManager =
      factory->defaultManagerForInterface(hostInterface, implFactory, logger);

  if (!defaultManager) {
    logger->info("No default manager configured");
    return 1;
  }

  logger->info(defaultManager->displayName());

  static const std::string kTraitId = "openassetio-mediacreation:content.LocatableContent";

  ContextPtr context = Context::make();

  {
    logger->info("Management Policy for " + kTraitId + " [read]:");
    context->access = Context::Access::kRead;
    const auto policies = defaultManager->managementPolicy({{kTraitId}}, context);
    for (const openassetio::trait::TraitId& id : policies[0]->traitSet()) {
      logger->info(id);
    }
  }
  {
    logger->info("Management Policy for " + kTraitId + " [write]:");
    context->access = Context::Access::kWrite;
    const auto policies = defaultManager->managementPolicy({{kTraitId}}, context);
    for (const openassetio::trait::TraitId& id : policies[0]->traitSet()) {
      logger->info(id);
    }
  }

  if (argc == 1) {
    logger->debug("Nothing to resolve");
    return 0;
  }

  EntityReference ref = defaultManager->createEntityReference(argv[1]);
  logger->info("Resolving " + ref.toString());

  context->access = Context::Access::kRead;
  defaultManager->resolve(
      {ref}, {kTraitId}, context,
      [&logger](std::size_t, const openassetio::TraitsDataPtr& data) {
        if (data->hasTrait(kTraitId)) {
          openassetio::trait::property::Value val;
          if (data->getTraitProperty(&val, kTraitId, "location")) {
            logger->info(kTraitId + ": " + std::get<openassetio::Str>(val));
          } else {
            logger->info(kTraitId + ": No location data");
          }
        } else {
          logger->info(kTraitId + ": Trait not imbued");
        }
      },
      [&logger](std::size_t, const openassetio::BatchElementError& error) {
        logger->error(error.message);
      });

  return 0;
}
