# SPDX-License-Identifier: Apache-2.0
# Copyright 2013-2023 The Foundry Visionmongers Ltd

set(SERVER_TARGET_NAME openassetio-grpc-server)

set(SRC main.cpp)

include(Common)

# Proto file
get_filename_component(openassetio_grpc_proto "../openassetio.proto" ABSOLUTE)
get_filename_component(openassetio_grpc_proto_path "${openassetio_grpc_proto}" PATH)

# Generated sources
set(openassetio_grpc_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/openassetio.pb.cc")
set(openassetio_grpc_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/openassetio.pb.h")
set(openassetio_grpc_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/openassetio.grpc.pb.cc")
set(openassetio_grpc_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/openassetio.grpc.pb.h")

add_custom_command(
      OUTPUT 
	  "${openassetio_grpc_proto_srcs}" "${openassetio_grpc_proto_hdrs}"
	  "${openassetio_grpc_grpc_srcs}" "${openassetio_grpc_grpc_hdrs}"
      COMMAND
	  ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${openassetio_grpc_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${openassetio_grpc_proto}"
      DEPENDS "${openassetio_grpc_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

add_library(openassetio_grpc_grpc_proto
  ${openassetio_grpc_grpc_srcs}
  ${openassetio_grpc_grpc_hdrs}
  ${openassetio_grpc_proto_srcs}
  ${openassetio_grpc_proto_hdrs})
target_link_libraries(openassetio_grpc_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

openassetio_grpc_set_default_target_properties(openassetio_grpc_grpc_proto)

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
