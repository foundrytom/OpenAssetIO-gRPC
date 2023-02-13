// SPDX-License-Identifier: Apache-2.0
// Copyright 2023 The Foundry Visionmongers Ltd
#pragma once

#include "openassetio-grpc/export.h"

#include <openassetio/managerApi/ManagerInterface.hpp>
#include <openassetio/typedefs.hpp>

OPENASSETIO_FWD_DECLARE(managerApi, HostSession)
OPENASSETIO_FWD_DECLARE(Context)

namespace openassetio::grpc {

class GRPCManagerInterfaceClient;

/**
 * An implementation of the OpenAssetIO MangerInterface that forwards
 * all methods to an out-of-process gRPC service.
 *
 * @note This class isn't currently intended for direct use. See the
 * GRPCManagerImplementationFactory instead.
 */
class OPENASSETIO_GRPC_EXPORT GRPCManagerInterface : public managerApi::ManagerInterface {
 public:

  using RemoteHandle = openassetio::Str;

  /**
   * @param handle The server-provided handle for the specific remote
   * interface instance to be wrapped. This string is supplied in the
   * InstantiateResponse message.
   *
   * @param channel The address/port of the service that holds the
   * instance referred to by handle.
   *
   * @todo Make these optional, so they can be supplied via manager
   * settings in `initialize`. This would allow this to then be
   * user-configured rather than relying on the gRPC specific factory.
   *
   * @todo HostInterface bridging is naive, in that we always re-send
   * the displayName/identifier with each request. This is a downside to
   * working at the ManagerInterface level - as we don't inherently know
   * if the host session is the same as last time. There are bunch of
   * ways we could address this, including creating a two-way bridge.
   */
  explicit GRPCManagerInterface(RemoteHandle handle, const std::string& channel);
  ~GRPCManagerInterface() override;

  Identifier identifier() const override;
  Str displayName() const override;

  void initialize(InfoDictionary managerSettings,
                  const managerApi::HostSessionPtr &hostSession) override;

  trait::TraitsDatas managementPolicy(
      const trait::TraitSets &traitSets, const ContextConstPtr &context,
      const managerApi::HostSessionPtr &hostSession) const override;

  bool isEntityReferenceString(const Str &someString,
                               const managerApi::HostSessionPtr &hostSession) const override;

  void resolve(const EntityReferences &entityReferences, const trait::TraitSet &traitSet,
               const ContextConstPtr &context, const managerApi::HostSessionPtr &hostSession,
               const ResolveSuccessCallback &successCallback,
               const BatchElementErrorCallback &errorCallback) override;

  void preflight(const EntityReferences &entityReferences, const trait::TraitSet &traitSet,
                 const ContextConstPtr &context, const managerApi::HostSessionPtr &hostSession,
                 const PreflightSuccessCallback &successCallback,
                 const BatchElementErrorCallback &errorCallback) override;

  void register_(const EntityReferences &entityReferences,
                 const trait::TraitsDatas &entityTraitsDatas, const ContextConstPtr &context,
                 const managerApi::HostSessionPtr &hostSession,
                 const RegisterSuccessCallback &successCallback,
                 const BatchElementErrorCallback &errorCallback) override;

 private:
  RemoteHandle handle_;
  std::unique_ptr<GRPCManagerInterfaceClient> client_;
};

}  // namespace openassetio::grpc
