# SPDX-License-Identifier: Apache-2.0
# Copyright 2013-2023 The Foundry Visionmongers Ltd

set(SERVER_TARGET_NAME openassetio-grpc-server)

set(SRC main.cpp)

add_executable(${SERVER_TARGET_NAME} ${SRC})

openassetio_grpc_set_default_target_properties(${SERVER_TARGET_NAME})

target_link_libraries(${SERVER_TARGET_NAME}
	openassetio_grpc_grpc_proto
	${_REFLECTION}
	${_GRPC_GRPCPP}
	${_PROTOBUF_LIBPROTOBUF})


install(
	TARGETS
	${SERVER_TARGET_NAME}
	DESTINATION
	.
	)
