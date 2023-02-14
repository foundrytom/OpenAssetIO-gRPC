// SPDX-License-Identifier: Apache-2.0
// Copyright 2013-2023 The Foundry Visionmongers Ltd
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

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
using openassetio::TraitsDataPtr;
using openassetio::hostApi::HostInterface;
using openassetio::hostApi::HostInterfacePtr;
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

class TestHostInterface : public HostInterface {
  [[nodiscard]] openassetio::Identifier identifier() const override {
    return "org.openassetio.gRPC.testHost";
  }

  [[nodiscard]] openassetio::Str displayName() const override {
    return "OpenAssetIO gRPC Test Host";
  }
};

static const std::string kIdentifierBAL = "org.openassetio.examples.manager.bal";
static const std::string kLocatableContentTraitId =
    "openassetio-mediacreation:content.LocatableContent";
static const std::string kManagedTraitId = "openassetio-mediacreation:managementPolicy.Managed";
static const std::string kAnEntityRefString = "bal:///anAsset";

SCENARIO("gRPC Manager Factory relays methods and returns expected response") {
  GIVEN("A ManagerFactory configured with the gRPC implementation factory") {
    auto logger = SeverityFilter::make(ConsoleLogger::make());
    auto implFactory =
        openassetio::grpc::GRPCManagerImplementationFactory::make("0.0.0.0:50051", logger);
    HostInterfacePtr hostInterface = std::make_shared<TestHostInterface>();
    ManagerFactoryPtr factory = ManagerFactory::make(hostInterface, implFactory, logger);

    WHEN("Available managers are queried") {
      const auto available = factory->availableManagers();

      THEN("BAL is available") {
        REQUIRE_FALSE(available.find(kIdentifierBAL) == available.end());
      }
      AND_THEN("BALs entry contains the corrent display name, identifier and info") {
        const auto& detail = available.find(kIdentifierBAL)->second;
        REQUIRE(detail.identifier == kIdentifierBAL);
        REQUIRE(detail.displayName == "Basic Asset Library 📖");
      }
    }

    AND_WHEN("The default manager is initialized") {
      ManagerPtr defaultManager =
          factory->defaultManagerForInterface(hostInterface, implFactory, logger);

      AND_THEN("The default manager is BAL") {
        REQUIRE(defaultManager->identifier() == kIdentifierBAL);
      }

      GIVEN("A suitable read context") {
        ContextPtr context = Context::make();
        context->access = Context::Access::kRead;

        WHEN("managementPolicy is queried for the LocatableContentTrait") {
          const auto policies =
              defaultManager->managementPolicy({{kLocatableContentTraitId}}, context);
          THEN("the trait is managed") {
            const auto traitSet = policies[0]->traitSet();
            REQUIRE_FALSE(traitSet.find(kManagedTraitId) == traitSet.end());
          }
        }

        AND_WHEN("The LocatableContentTrait is resolved for a test entity") {
          EntityReference ref = defaultManager->createEntityReference(kAnEntityRefString);

          TraitsDataPtr resolvedData;
          defaultManager->resolve(
              {ref}, {kLocatableContentTraitId}, context,
              [&resolvedData](const size_t /* unused */, const TraitsDataPtr& data) {
                resolvedData = data;
              },
              [](const size_t /* unused */, const openassetio::BatchElementError& error) {
                // Should never run this code path, use the comparison
                // to output the API error message to the shell to help
                // debug.
                REQUIRE(error.message == "<it will never be this>");
              });

          THEN("The location is the library value") {
            openassetio::trait::property::Value val;
            REQUIRE(resolvedData->getTraitProperty(&val, kLocatableContentTraitId, "location"));
            REQUIRE(std::get<std::string>(val) == "file:///dev/null");
          }
        }
      }
    }
  }
}
