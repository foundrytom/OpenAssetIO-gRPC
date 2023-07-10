from concurrent import futures
import logging

import grpc


from openassetio.log import ConsoleLogger

from . import openassetio_pb2_grpc

from .Server import Server

def serve():

    logger = ConsoleLogger()

    port = "51234"
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    openassetio_pb2_grpc.add_ManagerProxyServicer_to_server(Server(logger), server)
    server.add_insecure_port("[::]:" + port)
    server.start()
    print("Server started, listening on " + port)
    server.wait_for_termination()

logging.basicConfig()
serve()
