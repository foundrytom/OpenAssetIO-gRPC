import argparse
from concurrent import futures
import logging
import sys

import grpc


from openassetio.log import ConsoleLogger

from . import openassetio_pb2_grpc

from .Server import Server

def serve():

    parser = argparse.ArgumentParser(prog="openassetio-grpc-server")
    parser.add_argument("-d", "--with-shared-default-manager", action="store_true")
    parser.add_argument("-p", "--port", type=int, default=51234)

    args, _ = parser.parse_known_args(sys.argv)

    logger = ConsoleLogger()

    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    openassetio_pb2_grpc.add_ManagerProxyServicer_to_server(Server(logger,
                                                                   withDefaultInstance=args.with_shared_default_manager), server)
    server.add_insecure_port(f"[::]:{args.port}")
    server.start()
    print(f"Server started, listening on {args.port}")
    server.wait_for_termination()

logging.basicConfig()
serve()
