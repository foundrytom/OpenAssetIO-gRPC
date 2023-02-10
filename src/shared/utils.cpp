// SPDX-License-Identifier: Apache-2.0
// Copyright 2023 The Foundry Visionmongers Ltd
#include "utils.hpp"

#include <memory>

#include <openassetio/hostApi/HostInterface.hpp>
#include <openassetio/managerApi/Host.hpp>
#include <openassetio/managerApi/HostSession.hpp>
#include <openassetio/typedefs.hpp>
#include "openassetio.pb.h"

namespace {

class ProxyHostInterface : public openassetio::hostApi::HostInterface {
 public:
  ProxyHostInterface(openassetio::Identifier identifier, openassetio::Str displayName)
      : identifier_(std::move(identifier)), displayName_(std::move(displayName)) {}
  ~ProxyHostInterface() override = default;

  openassetio::Identifier identifier() const override { return identifier_; }
  openassetio::Str displayName() const override { return displayName_; }

 private:
  openassetio::Identifier identifier_;
  openassetio::Str displayName_;
};

}  // namespace

namespace openassetio::grpc {

InfoDictionary msgToInfoDictionary(const openassetio_grpc_proto::InfoDictMsg& msg) {
  InfoDictionary info;
  for (const auto& [key, value] : msg.bools()) {
    info.insert({key, value});
  }
  for (const auto& [key, value] : msg.ints()) {
    info.insert({key, value});
  }
  for (const auto& [key, value] : msg.floats()) {
    info.insert({key, value});
  }
  for (const auto& [key, value] : msg.strings()) {
    info.insert({key, value});
  }
  return info;
}

void infoDictionaryToMsg(const InfoDictionary& info, openassetio_grpc_proto::InfoDictMsg& outMsg) {
  auto* bools = outMsg.mutable_bools();
  auto* ints = outMsg.mutable_ints();
  auto* floats = outMsg.mutable_floats();
  auto* strings = outMsg.mutable_strings();

  for (const auto& [key, value] : info) {
    std::visit(
        [&key, &bools, &ints, &floats, &strings](auto&& arg) {
          using T = std::decay_t<decltype(arg)>;
          if constexpr (std::is_same_v<T, openassetio::Bool>) {
            bools->insert({key, arg});
          } else if constexpr (std::is_same_v<T, openassetio::Int>) {
            ints->insert({key, arg});
          } else if constexpr (std::is_same_v<T, openassetio::Float>) {
            floats->insert({key, arg});
          } else if constexpr (std::is_same_v<T, openassetio::Str>) {
            strings->insert({key, arg});
          }
        },
        value);
  }
}

managerApi::HostSessionPtr msgToHostSession(const openassetio_grpc_proto::HostSessionMsg& msg,
                                            log::LoggerInterfacePtr logger) {
  hostApi::HostInterfacePtr interface =
      std::make_shared<ProxyHostInterface>(msg.host().identifier(), msg.host().displayname());
  return managerApi::HostSession::make(managerApi::Host::make(interface), std::move(logger));
}

void hostSessionToMsg(const managerApi::HostSessionPtr& session,
                      openassetio_grpc_proto::HostSessionMsg* outMsg) {
  outMsg->mutable_host()->set_identifier(session->host()->identifier());
  outMsg->mutable_host()->set_displayname(session->host()->displayName());
}

}  // namespace openassetio::grpc
