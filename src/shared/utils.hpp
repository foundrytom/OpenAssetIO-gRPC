// SPDX-License-Identifier: Apache-2.0
// Copyright 2023 The Foundry Visionmongers Ltd
#pragma once

#include <openassetio/Context.hpp>
#include <openassetio/InfoDictionary.hpp>
#include <openassetio/TraitsData.hpp>
#include <openassetio/managerApi/HostSession.hpp>

#include "openassetio.grpc.pb.h"
#include "openassetio.pb.h"

/**
 * gRPC message <-> OpenAssetIO data converters
 */

namespace openassetio::grpc {

/*
* InfoDictionary
* @{
*/

InfoDictionary msgToInfoDictionary(const openassetio_grpc_proto::InfoDictMsg& msg);
void infoDictionaryToMsg(const InfoDictionary& info, openassetio_grpc_proto::InfoDictMsg* outMsg);

/*
* @}
*
* HostSession
* @{
*/

// Creates a proxy HostInterface that simply echos the host data
// included in the gRPC message.
managerApi::HostSessionPtr msgToHostSession(const openassetio_grpc_proto::HostSessionMsg& msg,
                                            log::LoggerInterfacePtr logger);
// Captures the required information from the HostSession so that it can
// be mocked on the server side.
void hostSessionToMsg(const managerApi::HostSessionPtr& session,
                      openassetio_grpc_proto::HostSessionMsg* outMsg);

/*
* @}
*
* Context
* @{
*/

ContextPtr msgToContext(const openassetio_grpc_proto::ContextMsg& msg);
void contextToMsg(const ContextConstPtr& context, openassetio_grpc_proto::ContextMsg* outMsg);

/*
* @}
*
* TraitsData
* @{
*/

TraitsDataPtr msgToTraitsData(const openassetio_grpc_proto::TraitsDataMsg& msg);
void traitsDataToMsg(const TraitsDataConstPtr& data,
                     openassetio_grpc_proto::TraitsDataMsg* outMsg);

/*
* @}
*
* TraitSet
* @{
*/


trait::TraitSet msgToTraitSet(const openassetio_grpc_proto::TraitSetMsg& msg);
void traitSetToMsg(const trait::TraitSet& set, openassetio_grpc_proto::TraitSetMsg* outMsg);

/**
* @}
*/

}  // namespace openassetio::grpc
