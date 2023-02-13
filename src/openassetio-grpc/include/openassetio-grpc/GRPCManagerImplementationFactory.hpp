// SPDX-License-Identifier: Apache-2.0
// Copyright 2013-2023 The Foundry Visionmongers Ltd
#pragma once

#include <memory>
#include <string>
#include <vector>

#include <openassetio/hostApi/ManagerImplementationFactoryInterface.hpp>
#include <openassetio/typedefs.hpp>

#include "openassetio-grpc/export.h"

namespace openassetio::grpc {

// Forward declare our implementation
class GRPCManagerImplementationFactoryClient;

OPENASSETIO_DECLARE_PTR(GRPCManagerImplementationFactory)

/**
 * Relay OpenAssetIO API requests our-of-process using gRPC/Protobuf.
 *
 * The GRPCManagerImplementationFactory provides an alternate
 * implementation factory for the OpenAssetIO ManagerFactory.
 *
 * It proxies to an out-of-process server to decouples the runtime
 * dependencies of any given Manager implementation from those of the
 * Host.
 *
 * Managers instantiated using this factory are gRPC wrappers to an
 * instance constructed within the remote server. Only the data
 * expressed through the OpenAssetIO interface contracts is exchanged,
 * providing runtime isolation between the specifics of the manager or
 * hosts respective implementations.
 */
class OPENASSETIO_GRPC_EXPORT GRPCManagerImplementationFactory final
    : public hostApi::ManagerImplementationFactoryInterface {
 public:
  /**
   * Constructs a new instance of the factory.
   *
   * @param channel The address/port of a server implementing the
   * openasseto-grpc service.
   *
   * @param logger A logger to use for all messages and debug info.
   */
  static GRPCManagerImplementationFactoryPtr make(const std::string& channel,
                                                  log::LoggerInterfacePtr logger);

  ~GRPCManagerImplementationFactory() override;

  [[nodiscard]] Identifiers identifiers() override;
  [[nodiscard]] managerApi::ManagerInterfacePtr instantiate(const Identifier& identifier) override;

 private:
  explicit GRPCManagerImplementationFactory(const std::string& channel,
                                            log::LoggerInterfacePtr logger);

  std::unique_ptr<GRPCManagerImplementationFactoryClient> client_;
  std::string channel_;
};

}  // namespace openassetio::grpc
