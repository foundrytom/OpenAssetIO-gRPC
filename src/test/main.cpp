// SPDX-License-Identifier: Apache-2.0
// Copyright 2013-2023 The Foundry Visionmongers Ltd
#include <iostream>

#include <memory>
#include <openassetio/hostApi/HostInterface.hpp>
#include <openassetio/hostApi/ManagerFactory.hpp>
#include <openassetio/log/ConsoleLogger.hpp>
#include <openassetio/log/SeverityFilter.hpp>

#include <openassetio-grpc/GRPCManagerImplementationFactory.hpp>

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

using openassetio::grpc::GRPCManagerImplementationFactory;
using openassetio::grpc::GRPCManagerImplementationFactoryPtr;

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
  openassetio::log::LoggerInterfacePtr logger = SeverityFilter::make(ConsoleLogger::make());

  GRPCManagerImplementationFactoryPtr implFactory =
      std::make_shared<GRPCManagerImplementationFactory>("0.0.0.0:50051", logger);

  ManagerFactoryPtr factory =
      ManagerFactory::make(std::make_shared<TestHostInterface>(), implFactory, logger);

  logger->info("availableManagers()");
  for (auto& [identifier, detail] : factory->availableManagers()) {
    logger->info(detail.displayName + " [" + detail.identifier + "]");
  }
  logger->info("Done");

  return 0;
}
