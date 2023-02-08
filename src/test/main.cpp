// SPDX-License-Identifier: Apache-2.0
// Copyright 2013-2023 The Foundry Visionmongers Ltd
#include <iostream>

#include <openassetio/log/ConsoleLogger.hpp>
#include <openassetio/log/SeverityFilter.hpp>

#include <openassetio-grpc/GRPCManagerImplementationFactory.hpp>

using openassetio::log::ConsoleLogger;
using openassetio::log::SeverityFilter;

using openassetio::grpc::GRPCManagerImplementationFactory;

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
  openassetio::log::LoggerInterfacePtr logger = SeverityFilter::make(ConsoleLogger::make());

  GRPCManagerImplementationFactory factory("0.0.0.0:50051", logger);

  logger->info("identifiers()");
  for (const std::string& identifier : factory.identifiers()) {
    logger->info(identifier);
  }
  logger->info("Done");

  return 0;
}
