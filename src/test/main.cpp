// SPDX-License-Identifier: Apache-2.0
// Copyright 2013-2023 The Foundry Visionmongers Ltd
#include <iostream>

#include <openassetio-grpc/GRPCManagerImplementationFactory.hpp>

using openassetio::grpc::GRPCManagerImplementationFactory;

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
  GRPCManagerImplementationFactory factory("0.0.0.0:50051");

  std::cout << "identifiers()" << std::endl;
  for (const std::string& identifier : factory.identifiers()) {
    std::cout << identifier << std::endl;
  }

  return 0;
}
