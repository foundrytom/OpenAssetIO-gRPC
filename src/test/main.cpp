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

/*
 * A very basic hacky host, along the lines of simpleResolver.py that
 * exercises the gRPC bridge.
 *
 * It inspects available managers, then initialises the default manager.
 * It's management policy is queried (then ignored), and if a single CLI
 * argument is supplied, it will attempt to treat that as an entity
 * reference, and resolve it with the LocatableContentTrait.
 */

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
  // Bootstrap API

  openassetio::log::LoggerInterfacePtr logger = SeverityFilter::make(ConsoleLogger::make());

  // Use the gRPC factory instead of the python one
  GRPCManagerImplementationFactoryPtr implFactory =
      GRPCManagerImplementationFactory::make("0.0.0.0:50051", logger);

  openassetio::hostApi::HostInterfacePtr hostInterface = std::make_shared<TestHostInterface>();

  ManagerFactoryPtr factory = ManagerFactory::make(hostInterface, implFactory, logger);

  // Introspect available managers (from the servers perspective)

  logger->info("Available managers:");
  for (auto& [identifier, detail] : factory->availableManagers()) {
    logger->info(detail.displayName + " [" + detail.identifier + "]");
  }

  // Initialize the default manager

  logger->info("Default manager:");
  ManagerPtr defaultManager =
      factory->defaultManagerForInterface(hostInterface, implFactory, logger);
  if (!defaultManager) {
    logger->info("  No default manager configured");
    return 1;
  }
  logger->info("  " + defaultManager->displayName());

  // Check managementPolicy

  ContextPtr context = Context::make();

  static const std::string kTraitId = "openassetio-mediacreation:content.LocatableContent";

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

  // Resolve the first arg if we have one

  if (argc == 1) {
    logger->debug("Nothing to resolve");
    return 0;
  }

  EntityReference ref = defaultManager->createEntityReference(argv[1]);
  logger->info("Resolving " + ref.toString());

  context->access = Context::Access::kRead;
  defaultManager->resolve(
      {ref}, {kTraitId}, context,
      [&logger](std::size_t /*unused*/, const openassetio::TraitsDataPtr& data) {
        // We really need C++ trait gen!
        if (data->hasTrait(kTraitId)) {
          openassetio::trait::property::Value val;
          if (data->getTraitProperty(&val, kTraitId, "location")) {
            // @todo Type check
            logger->info(kTraitId + ": " + std::get<openassetio::Str>(val));
          } else {
            logger->info(kTraitId + ": No location data");
          }
        } else {
          logger->info(kTraitId + ": Trait not imbued");
        }
      },
      [&logger](std::size_t /*unused*/, const openassetio::BatchElementError& error) {
        logger->error(error.message);
      });

  return 0;
}
