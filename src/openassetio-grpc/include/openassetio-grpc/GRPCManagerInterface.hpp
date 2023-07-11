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
 * @todo HostInterface bridging is naive, in that we always re-send
 * the displayName/identifier with each request. This is a downside to
 * working at the ManagerInterface level - as we don't inherently know
 * if the host session is the same as last time. There are bunch of
 * ways we could address this, including creating a two-way bridge.
 */
class OPENASSETIO_GRPC_EXPORT GRPCManagerInterface : public managerApi::ManagerInterface {
 public:
  using RemoteHandle = openassetio::Str;

  static const std::string kIdentifiergRPCManager;

  static const std::string kSettingChannel;
  static const std::string kSettingHandle;

  /**
   * Constructs an interface that can be used for generic connections to
   * an existing channel with a pre-initialized manager.
   *
   * In this mode, pre-initialization and initialization activities are
   * limited to configuring gRPC communications. If full control of the
   * target handle is needed, then the alternate constructor should be
   * used with `ownsHandle` set to true.
   *
   * This is suitable for use with the (future) C++ plugin system,
   * allowing any gRPC service implementing `openassetio.proto` to be
   * used as a manager in an un-modified host.
   */
  explicit GRPCManagerInterface();

  /**
   * @param handle The server-provided handle for the specific remote
   * interface instance to be wrapped. This string is supplied in the
   * InstantiateResponse message.
   *
   * When this constructor is used, the interface will operate in
   * 'transparent bridge' mode, and allow pre-initialization,
   * initialization and destruction of configured remote handle.
   *
   * This is used by the GRPCManagerImplementationFactory to proxy API
   * requests out-of-process.

   * @param channel The address/port of the service that holds the
   * instance referred to by handle.
   */
  explicit GRPCManagerInterface(RemoteHandle handle, const std::string &channel);

  ~GRPCManagerInterface() override;

  Identifier identifier() const override;
  Str displayName() const override;
  InfoDictionary info() const override;

  InfoDictionary settings(const managerApi::HostSessionPtr &hostSession) const override;

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
  InfoDictionary settings_;
  bool ownsHandle_;
};

}  // namespace openassetio::grpc
