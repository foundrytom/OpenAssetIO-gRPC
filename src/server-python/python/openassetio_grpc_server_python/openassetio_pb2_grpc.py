# Generated by the gRPC Python protocol compiler plugin. DO NOT EDIT!
"""Client and server classes corresponding to protobuf-defined services."""
import grpc

from . import openassetio_pb2 as openassetio__pb2


class ManagerProxyStub(object):
    """
    Service definition

    The same service covers both factory and interface methods.

    The service assigns a unique handle to each manager implementation
    that it instantiates. This is returned in the InstantiateResponse,
    and must be supplied as the handle parameter in all ManagerInterface
    method requests so that they are routed to the correct instance.
    """

    def __init__(self, channel):
        """Constructor.

        Args:
            channel: A grpc.Channel.
        """
        self.Identifiers = channel.unary_unary(
                '/openassetio_grpc_proto.ManagerProxy/Identifiers',
                request_serializer=openassetio__pb2.EmptyRequest.SerializeToString,
                response_deserializer=openassetio__pb2.IdentifiersResponse.FromString,
                )
        self.Instantiate = channel.unary_unary(
                '/openassetio_grpc_proto.ManagerProxy/Instantiate',
                request_serializer=openassetio__pb2.InstantiateRequest.SerializeToString,
                response_deserializer=openassetio__pb2.InstantiateResponse.FromString,
                )
        self.Identifier = channel.unary_unary(
                '/openassetio_grpc_proto.ManagerProxy/Identifier',
                request_serializer=openassetio__pb2.IdentifierRequest.SerializeToString,
                response_deserializer=openassetio__pb2.IdentifierResponse.FromString,
                )
        self.DisplayName = channel.unary_unary(
                '/openassetio_grpc_proto.ManagerProxy/DisplayName',
                request_serializer=openassetio__pb2.DisplayNameRequest.SerializeToString,
                response_deserializer=openassetio__pb2.DisplayNameResponse.FromString,
                )
        self.Info = channel.unary_unary(
                '/openassetio_grpc_proto.ManagerProxy/Info',
                request_serializer=openassetio__pb2.InfoRequest.SerializeToString,
                response_deserializer=openassetio__pb2.InfoResponse.FromString,
                )
        self.Initialize = channel.unary_unary(
                '/openassetio_grpc_proto.ManagerProxy/Initialize',
                request_serializer=openassetio__pb2.InitializeRequest.SerializeToString,
                response_deserializer=openassetio__pb2.EmptyResponse.FromString,
                )
        self.ManagementPolicy = channel.unary_unary(
                '/openassetio_grpc_proto.ManagerProxy/ManagementPolicy',
                request_serializer=openassetio__pb2.ManagementPolicyRequest.SerializeToString,
                response_deserializer=openassetio__pb2.ManagementPolicyResponse.FromString,
                )
        self.IsEntityReferenceString = channel.unary_unary(
                '/openassetio_grpc_proto.ManagerProxy/IsEntityReferenceString',
                request_serializer=openassetio__pb2.IsEntityReferenceStringRequest.SerializeToString,
                response_deserializer=openassetio__pb2.IsEntityReferenceStringResponse.FromString,
                )
        self.Resolve = channel.unary_unary(
                '/openassetio_grpc_proto.ManagerProxy/Resolve',
                request_serializer=openassetio__pb2.ResolveRequest.SerializeToString,
                response_deserializer=openassetio__pb2.ResolveResponse.FromString,
                )
        self.Preflight = channel.unary_unary(
                '/openassetio_grpc_proto.ManagerProxy/Preflight',
                request_serializer=openassetio__pb2.PreflightRequest.SerializeToString,
                response_deserializer=openassetio__pb2.PreflightResponse.FromString,
                )
        self.Register = channel.unary_unary(
                '/openassetio_grpc_proto.ManagerProxy/Register',
                request_serializer=openassetio__pb2.RegisterRequest.SerializeToString,
                response_deserializer=openassetio__pb2.RegisterResponse.FromString,
                )
        self.Destroy = channel.unary_unary(
                '/openassetio_grpc_proto.ManagerProxy/Destroy',
                request_serializer=openassetio__pb2.DestroyRequest.SerializeToString,
                response_deserializer=openassetio__pb2.EmptyResponse.FromString,
                )


class ManagerProxyServicer(object):
    """
    Service definition

    The same service covers both factory and interface methods.

    The service assigns a unique handle to each manager implementation
    that it instantiates. This is returned in the InstantiateResponse,
    and must be supplied as the handle parameter in all ManagerInterface
    method requests so that they are routed to the correct instance.
    """

    def Identifiers(self, request, context):
        """MawnagerImplementationFactoryInterface methods
        """
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def Instantiate(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def Identifier(self, request, context):
        """ManagerInterface methods
        """
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def DisplayName(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def Info(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def Initialize(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def ManagementPolicy(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def IsEntityReferenceString(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def Resolve(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def Preflight(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def Register(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def Destroy(self, request, context):
        """Lifetime management
        """
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')


def add_ManagerProxyServicer_to_server(servicer, server):
    rpc_method_handlers = {
            'Identifiers': grpc.unary_unary_rpc_method_handler(
                    servicer.Identifiers,
                    request_deserializer=openassetio__pb2.EmptyRequest.FromString,
                    response_serializer=openassetio__pb2.IdentifiersResponse.SerializeToString,
            ),
            'Instantiate': grpc.unary_unary_rpc_method_handler(
                    servicer.Instantiate,
                    request_deserializer=openassetio__pb2.InstantiateRequest.FromString,
                    response_serializer=openassetio__pb2.InstantiateResponse.SerializeToString,
            ),
            'Identifier': grpc.unary_unary_rpc_method_handler(
                    servicer.Identifier,
                    request_deserializer=openassetio__pb2.IdentifierRequest.FromString,
                    response_serializer=openassetio__pb2.IdentifierResponse.SerializeToString,
            ),
            'DisplayName': grpc.unary_unary_rpc_method_handler(
                    servicer.DisplayName,
                    request_deserializer=openassetio__pb2.DisplayNameRequest.FromString,
                    response_serializer=openassetio__pb2.DisplayNameResponse.SerializeToString,
            ),
            'Info': grpc.unary_unary_rpc_method_handler(
                    servicer.Info,
                    request_deserializer=openassetio__pb2.InfoRequest.FromString,
                    response_serializer=openassetio__pb2.InfoResponse.SerializeToString,
            ),
            'Initialize': grpc.unary_unary_rpc_method_handler(
                    servicer.Initialize,
                    request_deserializer=openassetio__pb2.InitializeRequest.FromString,
                    response_serializer=openassetio__pb2.EmptyResponse.SerializeToString,
            ),
            'ManagementPolicy': grpc.unary_unary_rpc_method_handler(
                    servicer.ManagementPolicy,
                    request_deserializer=openassetio__pb2.ManagementPolicyRequest.FromString,
                    response_serializer=openassetio__pb2.ManagementPolicyResponse.SerializeToString,
            ),
            'IsEntityReferenceString': grpc.unary_unary_rpc_method_handler(
                    servicer.IsEntityReferenceString,
                    request_deserializer=openassetio__pb2.IsEntityReferenceStringRequest.FromString,
                    response_serializer=openassetio__pb2.IsEntityReferenceStringResponse.SerializeToString,
            ),
            'Resolve': grpc.unary_unary_rpc_method_handler(
                    servicer.Resolve,
                    request_deserializer=openassetio__pb2.ResolveRequest.FromString,
                    response_serializer=openassetio__pb2.ResolveResponse.SerializeToString,
            ),
            'Preflight': grpc.unary_unary_rpc_method_handler(
                    servicer.Preflight,
                    request_deserializer=openassetio__pb2.PreflightRequest.FromString,
                    response_serializer=openassetio__pb2.PreflightResponse.SerializeToString,
            ),
            'Register': grpc.unary_unary_rpc_method_handler(
                    servicer.Register,
                    request_deserializer=openassetio__pb2.RegisterRequest.FromString,
                    response_serializer=openassetio__pb2.RegisterResponse.SerializeToString,
            ),
            'Destroy': grpc.unary_unary_rpc_method_handler(
                    servicer.Destroy,
                    request_deserializer=openassetio__pb2.DestroyRequest.FromString,
                    response_serializer=openassetio__pb2.EmptyResponse.SerializeToString,
            ),
    }
    generic_handler = grpc.method_handlers_generic_handler(
            'openassetio_grpc_proto.ManagerProxy', rpc_method_handlers)
    server.add_generic_rpc_handlers((generic_handler,))


 # This class is part of an EXPERIMENTAL API.
class ManagerProxy(object):
    """
    Service definition

    The same service covers both factory and interface methods.

    The service assigns a unique handle to each manager implementation
    that it instantiates. This is returned in the InstantiateResponse,
    and must be supplied as the handle parameter in all ManagerInterface
    method requests so that they are routed to the correct instance.
    """

    @staticmethod
    def Identifiers(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/openassetio_grpc_proto.ManagerProxy/Identifiers',
            openassetio__pb2.EmptyRequest.SerializeToString,
            openassetio__pb2.IdentifiersResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def Instantiate(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/openassetio_grpc_proto.ManagerProxy/Instantiate',
            openassetio__pb2.InstantiateRequest.SerializeToString,
            openassetio__pb2.InstantiateResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def Identifier(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/openassetio_grpc_proto.ManagerProxy/Identifier',
            openassetio__pb2.IdentifierRequest.SerializeToString,
            openassetio__pb2.IdentifierResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def DisplayName(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/openassetio_grpc_proto.ManagerProxy/DisplayName',
            openassetio__pb2.DisplayNameRequest.SerializeToString,
            openassetio__pb2.DisplayNameResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def Info(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/openassetio_grpc_proto.ManagerProxy/Info',
            openassetio__pb2.InfoRequest.SerializeToString,
            openassetio__pb2.InfoResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def Initialize(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/openassetio_grpc_proto.ManagerProxy/Initialize',
            openassetio__pb2.InitializeRequest.SerializeToString,
            openassetio__pb2.EmptyResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def ManagementPolicy(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/openassetio_grpc_proto.ManagerProxy/ManagementPolicy',
            openassetio__pb2.ManagementPolicyRequest.SerializeToString,
            openassetio__pb2.ManagementPolicyResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def IsEntityReferenceString(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/openassetio_grpc_proto.ManagerProxy/IsEntityReferenceString',
            openassetio__pb2.IsEntityReferenceStringRequest.SerializeToString,
            openassetio__pb2.IsEntityReferenceStringResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def Resolve(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/openassetio_grpc_proto.ManagerProxy/Resolve',
            openassetio__pb2.ResolveRequest.SerializeToString,
            openassetio__pb2.ResolveResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def Preflight(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/openassetio_grpc_proto.ManagerProxy/Preflight',
            openassetio__pb2.PreflightRequest.SerializeToString,
            openassetio__pb2.PreflightResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def Register(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/openassetio_grpc_proto.ManagerProxy/Register',
            openassetio__pb2.RegisterRequest.SerializeToString,
            openassetio__pb2.RegisterResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def Destroy(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/openassetio_grpc_proto.ManagerProxy/Destroy',
            openassetio__pb2.DestroyRequest.SerializeToString,
            openassetio__pb2.EmptyResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)
