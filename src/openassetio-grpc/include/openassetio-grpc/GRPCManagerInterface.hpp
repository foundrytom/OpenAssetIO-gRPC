// SPDX-License-Identifier: Apache-2.0
// Copyright 2023 The Foundry Visionmongers Ltd
#pragma once

#include "openassetio-grpc/export.h"

#include <openassetio/managerApi/ManagerInterface.hpp>
#include <openassetio/typedefs.hpp>

OPENASSETIO_FWD_DECLARE(managerApi, HostSession)
OPENASSETIO_FWD_DECLARE(Context)

namespace openassetio::grpc {

// Forward declare client class
class GRPCManagerInterfaceClient;

class OPENASSETIO_GRPC_EXPORT GRPCManagerInterface : public managerApi::ManagerInterface {
 public:
  using RemoteHandle = openassetio::Str;

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
