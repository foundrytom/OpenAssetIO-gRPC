// SPDX-License-Identifier: Apache-2.0
// Copyright 2023 The Foundry Visionmongers Ltd
#pragma once

#include <openassetio/InfoDictionary.hpp>
#include <openassetio/managerApi/HostSession.hpp>

#include "openassetio.grpc.pb.h"

namespace openassetio::grpc {

InfoDictionary msgToInfoDictionary(const openassetio_grpc_proto::InfoDictMsg& msg);
void infoDictionaryToMsg(const InfoDictionary& info, openassetio_grpc_proto::InfoDictMsg* outMsg);

managerApi::HostSessionPtr msgToHostSession(const openassetio_grpc_proto::HostSessionMsg& msg,
                                            log::LoggerInterfacePtr logger);
void hostSessionToMsg(const managerApi::HostSessionPtr& session,
                      openassetio_grpc_proto::HostSessionMsg* outMsg);

}  // namespace openassetio::grpc
