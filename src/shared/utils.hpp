// SPDX-License-Identifier: Apache-2.0
// Copyright 2023 The Foundry Visionmongers Ltd
#pragma once

#include <openassetio/Context.hpp>
#include <openassetio/InfoDictionary.hpp>
#include <openassetio/TraitsData.hpp>
#include <openassetio/managerApi/HostSession.hpp>

#include "openassetio.grpc.pb.h"
#include "openassetio.pb.h"

namespace openassetio::grpc {

InfoDictionary msgToInfoDictionary(const openassetio_grpc_proto::InfoDictMsg& msg);
void infoDictionaryToMsg(const InfoDictionary& info, openassetio_grpc_proto::InfoDictMsg* outMsg);

managerApi::HostSessionPtr msgToHostSession(const openassetio_grpc_proto::HostSessionMsg& msg,
                                            log::LoggerInterfacePtr logger);
void hostSessionToMsg(const managerApi::HostSessionPtr& session,
                      openassetio_grpc_proto::HostSessionMsg* outMsg);

ContextPtr msgToContext(const openassetio_grpc_proto::ContextMsg& msg);
void contextToMsg(const ContextConstPtr& context, openassetio_grpc_proto::ContextMsg* outMsg);

TraitsDataPtr msgToTraitsData(const openassetio_grpc_proto::TraitsDataMsg& msg);
void traitsDataToMsg(const TraitsDataConstPtr& data,
                     openassetio_grpc_proto::TraitsDataMsg* outMsg);

trait::TraitSet msgToTraitSet(const openassetio_grpc_proto::TraitSetMsg& msg);
void traitSetToMsg(const trait::TraitSet& set, openassetio_grpc_proto::TraitSetMsg* outMsg);
}  // namespace openassetio::grpc
