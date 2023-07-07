# SPDX-License-Identifier: Apache-2.0
# Copyright 2023 The Foundry Visionmongers Ltd
"""
 The server is a very simplistic, hacky gRPC unary gRPC server based
 off the quickstart guides.

 It ignores the normal Manager/ManagerFactory layer of OpenAssetIO and
 works directly at the lower level by providing external access to a
 ManagerImplementationFactoryInterface, and then the corresponding
 methods of any instantiated ManagerInterfaces.

 This greatly simplifies proxying methods to a Host, as its tasks are
 then two fold:

  - Instantiate ManagerInterfaces on demand, assign them a handle, and
    keep them alive until otherwise requested.

  - Convert incoming gRPC messages to their equivalent OpenAssetIO
    data types, invoke the methods on their corresponding
    ManagerInterface instance, convert the result to gPRC messages for
    the response.

 At present, instantiated managers are kept in a map, using their memory
 address as a unique key and as the handle used in gRPC messages.

 @todo This needs a lot of tidying and work on error handling for
 anything other than the happy path.
"""
from os import wait
from functools import wraps
import uuid

import grpc

from openassetio.pluginSystem import PythonPluginSystemManagerImplementationFactory

from . import openassetio_pb2
from . import openassetio_pb2_grpc
from . import utils

# pylint: disable=broad-except


def uses_manager(func):
    """
    A decorator for gRPC server methods that will retrieve the manager
    instance based on the handle in the request, or except in the case
    of an unknown handle.

    It will append a positional argument with the manager to the wrapped
    function.
    """
    @wraps(func)
    def run(server, request, context, *args, **kwargs):
        handle = request.handle
        manager = server.managers.get(request.handle)
        if not manager:
            raise RuntimeError(f"{func.__name__}: Unknown handle {handle}")
        server.logger.debugApi(f"{func.__name__}() {handle}")
        return func(server, request, context, manager, *args, **kwargs)

    return run


def uses_host_session(func):
    """
    A decorator for gRPC server methods that will decode the HostSession
    information from the request and pass it to the method as an
    additional positional argument.
    """
    @wraps(func)
    def run(server, request, context, *args, **kwargs):
        host_session = utils.message_to_host_session(request.hostSession, server.logger)
        return func(server, request, context, host_session, *args, **kwargs)

    return run


def uses_context(func):
    """
    A decorator for gRPC server methods that will decode the
    (OpenAssetIO) Context from the request and pass it to
    the method as an additional positional argument.
    """
    @wraps(func)
    def run(server, request, context, *args, **kwargs):
        openassetio_context = utils.message_to_context(request.context)
        return func(server, request, context, openassetio_context, *args, **kwargs)

    return run


class Server(openassetio_pb2_grpc.ManagerProxyServicer):
    """
    An implementation of the OpenAssetIO gRPC Server.

    ManagerInterface instances are held in a dict, keyed by their handle.
    """
    def __init__(self, logger):
        super().__init__()

        self.logger = logger
        self.managers = {}

        self.__implementation_factory = PythonPluginSystemManagerImplementationFactory(logger)

    ## ManagerFactory methods

    def Identifiers(self, request, context):
        return openassetio_pb2.IdentifiersResponse(
            identifier=self.__implementation_factory.identifiers()
        )

    def Instantiate(self, request, context):
        identifier = request.identifier
        manager_interface = self.__implementation_factory.instantiate(identifier)
        handle = str(uuid.uuid4())
        self.managers[handle] = manager_interface
        self.logger.debugApi(f"Instantiated {identifier} with handle {handle}")
        return openassetio_pb2.InstantiateResponse(handle=handle)

    def Destroy(self, request, context):
        handle = request.handle
        if handle in self.managers:
            del self.managers[handle]
            self.logger.debugApi(f"Destroyed {handle}")
        else:
            self.logger.warning(f"Requested to destroy non-existent handle {handle}")
        return openassetio_pb2.EmptyResponse()

    ## ManagerInterface methods

    @uses_manager
    def Identifier(self, request, context, manager):
        return openassetio_pb2.IdentifierResponse(identifier=manager.identifier())

    @uses_manager
    def DisplayName(self, request, context, manager):
        return openassetio_pb2.DisplayNameResponse(displayName=manager.displayName())

    @uses_manager
    def Info(self, request, context, manager):
        return openassetio_pb2.InfoResponse(info=utils.info_dict_to_message(manager.info()))

    @uses_host_session
    @uses_manager
    def Initialize(self, request, context, manager, host_session):
        manager_settings = utils.info_message_to_dict(request.settings)
        manager.initialize(manager_settings, host_session)
        return openassetio_pb2.EmptyResponse()

    @uses_context
    @uses_host_session
    @uses_manager
    def ManagementPolicy(self, request, context, manager, host_session, openassetio_context):
        trait_sets = [utils.message_to_trait_set(msg) for msg in request.traitSet]
        policies = manager.managementPolicy(trait_sets, openassetio_context, host_session)
        return openassetio_pb2.ManagementPolicyResponse(
            policy=[utils.traits_data_to_message(p) for p in policies]
        )

    @uses_host_session
    @uses_manager
    def IsEntityReferenceString(
        self, request, context, manager, host_session,
    ):
        kwargs = { "is": manager.isEntityReferenceString(request.someString, host_session) }
        return openassetio_pb2.IsEntityReferenceStringResponse(**kwargs)

    @uses_context
    @uses_host_session
    @uses_manager
    def Resolve(self, request, context, manager, host_session, openassetio_context):
        result = []

        def success(idx, traits_data):
            result.append(
                openassetio_pb2.ResolveResponse.ResultOrErrorMsg(
                    index=idx, result=utils.traits_data_to_message(traits_data)
                )
            )

        def error(idx, batch_element_error):
            result.append(
                openassetio_pb2.ResolveResponse.ResultOrErrorMsg(
                    index=idx, result=utils.batch_element_error_to_message(batch_element_error)
                )
            )

        refs = utils.to_entity_refs(request.entityReference, manager, host_session)
        manager.resolve(
            refs,
            utils.message_to_trait_set(request.traitSet),
            openassetio_context,
            host_session,
            success,
            error,
        )
        return openassetio_pb2.ResolveResponse(resultOrError=result)

    @uses_context
    @uses_host_session
    @uses_manager
    def Preflight(self, request, context, manager, host_session, openassetio_context):
        result = []

        def success(idx, entity_reference):
            result.append(
                openassetio_pb2.PreflightResponse.ResultOrErrorMsg(
                    index=idx, result=entity_reference.toString()
                )
            )

        def error(idx, batch_element_error):
            result.append(
                openassetio_pb2.PreflightResponse.ResultOrErrorMsg(
                    index=idx, result=utils.batch_element_error_to_message(batch_element_error)
                )
            )

        refs = utils.to_entity_refs(request.entityReference, manager, host_session)
        manager.preflight(
            refs,
            utils.message_to_trait_set(request.traitSet),
            openassetio_context,
            host_session,
            success,
            error,
        )
        return openassetio_pb2.PreflightResponse(resultOrError=result)

    @uses_context
    @uses_host_session
    @uses_manager
    def Register(self, request, context, manager, host_session, openassetio_context):
        result = []

        def success(idx, entity_reference):
            result.append(
                openassetio_pb2.RegisterResponse.ResultOrErrorMsg(
                    index=idx, result=entity_reference.toString()
                )
            )

        def error(idx, batch_element_error):
            result.append(
                openassetio_pb2.RegisterResponse.ResultOrErrorMsg(
                    index=idx, result=utils.batch_element_error_to_message(batch_element_error)
                )
            )

        refs = utils.to_entity_refs(request.entityReference, manager, host_session)
        datas = [utils.message_to_traits_data(m) for m in request.traitsData]
        manager.register(
            refs,
            datas,
            openassetio_context,
            host_session,
            success,
            error,
        )
        return openassetio_pb2.RegisterResponse(resultOrError=result)
