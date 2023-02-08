# SPDX-License-Identifier: Apache-2.0
# Copyright 2013-2023 The Foundry Visionmongers Ltd

# Fine OpenAssetIO as a CMake package
find_package(OpenAssetIO REQUIRED)

# Find Protobuf installation Looks for protobuf-config.cmake file installed by
# Protobuf's cmake installation.
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

# Find gRPC installation Looks for gRPCConfig.cmake file installed by gRPC's
# cmake installation.
find_package(gRPC REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP gRPC::grpc++)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

# Proto file
get_filename_component(openassetio_grpc_proto "./openassetio.proto" ABSOLUTE)
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

add_library(openassetio_grpc_grpc_proto STATIC
  ${openassetio_grpc_grpc_srcs}
  ${openassetio_grpc_grpc_hdrs}
  ${openassetio_grpc_proto_srcs}
  ${openassetio_grpc_proto_hdrs})
target_link_libraries(openassetio_grpc_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

target_compile_definitions(openassetio_grpc_grpc_proto PRIVATE _GLIBCXX_USE_CXX11_ABI=0)

add_subdirectory(server)
add_subdirectory(openassetio-grpc)
add_subdirectory(test)
