// SPDX-License-Identifier: Apache-2.0
// Copyright 2013-2023 The Foundry Visionmongers Ltd
#include <iostream>

#include <memory>
#include <openassetio/hostApi/HostInterface.hpp>
#include <openassetio/hostApi/Manager.hpp>
#include <openassetio/hostApi/ManagerFactory.hpp>
#include <openassetio/log/ConsoleLogger.hpp>
#include <openassetio/log/SeverityFilter.hpp>

#include <openassetio-grpc/GRPCManagerImplementationFactory.hpp>

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

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  openassetio::log::LoggerInterfacePtr logger = SeverityFilter::make(ConsoleLogger::make());

  GRPCManagerImplementationFactoryPtr implFactory =
      std::make_shared<GRPCManagerImplementationFactory>("0.0.0.0:50051", logger);

  openassetio::hostApi::HostInterfacePtr hostInterface = std::make_shared<TestHostInterface>();

  ManagerFactoryPtr factory = ManagerFactory::make(hostInterface, implFactory, logger);
  logger->info("Available managers:");
  for (auto &[identifier, detail] : factory->availableManagers()) {
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

  return 0;
}
