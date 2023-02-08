// SPDX-License-Identifier: Apache-2.0
// Copyright 2013-2023 The Foundry Visionmongers Ltd
#pragma once

#include <memory>
#include <string>
#include <vector>

#include <openassetio/typedefs.hpp>
#include <openassetio/hostApi/ManagerImplementationFactoryInterface.hpp>

#include "openassetio-grpc/export.h"

namespace openassetio::grpc {

// Forward declare our implementation
class GRPCManagerImplementationFactoryClient;

OPENASSETIO_DECLARE_PTR(GRPCManagerImplementationFactory)

/*
 * Relay OpenAssetIO API requests our-of-process using gRPC/Protobuf.
 *
 *
 * The GRPCManagerImplementationFactory provides an alternate
 * implementation factory for the OpenAssetIO ManagerFactory.
 *
 * It proxies to an out-of-process server to decouples the runtime
 * dependencies of any given Manager implementation from those of the
 * Host.
 *
 * Managers instantiated using this factory are gRPC wrappers to an
 * instance constructed within the remote server.
 */

class OPENASSETIO_GRPC_EXPORT GRPCManagerImplementationFactory final
    : public hostApi::ManagerImplementationFactoryInterface {
 public:
  explicit GRPCManagerImplementationFactory(const std::string& channel,
                                            log::LoggerInterfacePtr logger);

  ~GRPCManagerImplementationFactory() override;

  [[nodiscard]] Identifiers identifiers() override;
  [[nodiscard]] managerApi::ManagerInterfacePtr instantiate(const Identifier& identifier) override;

 private:
  std::unique_ptr<GRPCManagerImplementationFactoryClient> client_;
  std::string channel_;
};

}  // namespace openassetio::grpc
