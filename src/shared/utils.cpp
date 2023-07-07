// SPDX-License-Identifier: Apache-2.0
// Copyright 2023 The Foundry Visionmongers Ltd
#include "utils.hpp"

#include <memory>

#include <openassetio/hostApi/HostInterface.hpp>
#include <openassetio/managerApi/Host.hpp>
#include <openassetio/managerApi/HostSession.hpp>
#include <openassetio/managerApi/ManagerInterface.hpp>
#include <openassetio/trait/collection.hpp>
#include <openassetio/trait/property.hpp>
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

// InfoDictionary

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

void infoDictionaryToMsg(const InfoDictionary& info, openassetio_grpc_proto::InfoDictMsg* outMsg) {
  auto* bools = outMsg->mutable_bools();
  auto* ints = outMsg->mutable_ints();
  auto* floats = outMsg->mutable_floats();
  auto* strings = outMsg->mutable_strings();

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

// HostSession

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

// Context

ContextPtr msgToContext(const openassetio_grpc_proto::ContextMsg& msg) {
  auto context = Context::make(Context::Access(msg.access()), Context::Retention(msg.retention()),
                       msgToTraitsData(msg.locale()),
                       nullptr  // TODO(tc) add state bridge
  );
  // Ensure the manager can determine it is being proxied.
  context->locale->addTrait("openassetio-grpc:locale.GRPCProxy");
  return context;
}

void contextToMsg(const ContextConstPtr& context, openassetio_grpc_proto::ContextMsg* outMsg) {
  outMsg->set_access(int32_t(context->access));
  outMsg->set_retention(int32_t(context->retention));
  traitsDataToMsg(context->locale, outMsg->mutable_locale());
}

// TraitsData

TraitsDataPtr msgToTraitsData(const openassetio_grpc_proto::TraitsDataMsg& msg) {
  TraitsDataPtr data = TraitsData::make();
  for (const auto& trait : msg.trait()) {
    const auto& traitId = trait.id();
    for (const auto& [key, value] : trait.bools()) {
      data->setTraitProperty(traitId, key, value);
    }
    for (const auto& [key, value] : trait.ints()) {
      data->setTraitProperty(traitId, key, value);
    }
    for (const auto& [key, value] : trait.floats()) {
      data->setTraitProperty(traitId, key, value);
    }
    for (const auto& [key, value] : trait.strings()) {
      data->setTraitProperty(traitId, key, value);
    }
  }
  return data;
}

void traitsDataToMsg(const TraitsDataConstPtr& data,
                     openassetio_grpc_proto::TraitsDataMsg* outMsg) {
  if(!data) { return; }
  for (const trait::TraitId& traitId : data->traitSet()) {
    auto* traitMsg = outMsg->add_trait();
    traitMsg->set_id(traitId);
    auto* bools = traitMsg->mutable_bools();
    auto* ints = traitMsg->mutable_ints();
    auto* floats = traitMsg->mutable_floats();
    auto* strings = traitMsg->mutable_strings();
    for (const Str& property : data->traitPropertyKeys(traitId)) {
      trait::property::Value value;
      if (data->getTraitProperty(&value, traitId, property)) {
        std::visit(
            [&property, &bools, &ints, &floats, &strings](auto&& arg) {
              using T = std::decay_t<decltype(arg)>;
              if constexpr (std::is_same_v<T, openassetio::Bool>) {
                bools->insert({property, arg});
              } else if constexpr (std::is_same_v<T, openassetio::Int>) {
                ints->insert({property, arg});
              } else if constexpr (std::is_same_v<T, openassetio::Float>) {
                floats->insert({property, arg});
              } else if constexpr (std::is_same_v<T, openassetio::Str>) {
                strings->insert({property, arg});
              }
            },
            value);
      }
    }
  }
}

// TraitSet

trait::TraitSet msgToTraitSet(const openassetio_grpc_proto::TraitSetMsg& msg) {
  trait::TraitSet set;
  for(const trait::TraitId& id: msg.traitid()) {
    set.insert(id);
  }
  return set;
}

void traitSetToMsg(const trait::TraitSet& set, openassetio_grpc_proto::TraitSetMsg* outMsg) {
  for(const trait::TraitId& id: set) {
    *(outMsg->add_traitid()) = id;
  }
}

}  // namespace openassetio::grpc
