// SPDX-License-Identifier: Apache-2.0
// Copyright 2013-2023 The Foundry Visionmongers Ltd
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "openassetio-grpc/export.h"

namespace openassetio::grpc {

class GRPCManagerImplementationFactoryClient;

class OPENASSETIO_GRPC_EXPORT GRPCManagerImplementationFactory final {
 public:
  explicit GRPCManagerImplementationFactory(const std::string& channel);
  ~GRPCManagerImplementationFactory();

  [[nodiscard]] std::vector<std::string> identifiers();

 private:
  std::unique_ptr<GRPCManagerImplementationFactoryClient> client_;
};

}  // namespace openassetio::grpc
