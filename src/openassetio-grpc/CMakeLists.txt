# SPDX-License-Identifier: Apache-2.0
# Copyright 2013-2023 The Foundry Visionmongers Ltd

set(LIB_TARGET_NAME openassetio-grpc)

# -------------------------------------------------------------------------------------------------
# Public Headers

set(public_header_source_root ${CMAKE_CURRENT_LIST_DIR}/include)

install(
    DIRECTORY
    ${public_header_source_root}/openassetio-grpc
    DESTINATION
    ${CMAKE_INSTALL_INCLUDEDIR}
)

# -------------------------------------------------------------------------------------------------
# Lib Target

add_library(${LIB_TARGET_NAME})
add_library(${PROJECT_NAME}::${LIB_TARGET_NAME} ALIAS ${LIB_TARGET_NAME})

openassetio_grpc_set_default_target_properties(${LIB_TARGET_NAME})

set_target_properties(${LIB_TARGET_NAME}
    PROPERTIES
    OUTPUT_NAME openassetio-grpc)

install(
    TARGETS
    ${LIB_TARGET_NAME}
    EXPORT ${PROJECT_NAME}_EXPORTED_TARGETS)


# -------------------------------------------------------------------------------------------------
# Lib Target Dependencies

target_sources(
    ${LIB_TARGET_NAME}
    PRIVATE
    src/GRPCManagerImplementationFactory.cpp)

target_include_directories(
    ${LIB_TARGET_NAME}
    PUBLIC
    # For generated export.h header.
    "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>"
    # Use includes from source tree for building.
    "$<BUILD_INTERFACE:${public_header_source_root}>"
    # Use includes from install tree for installed lib.
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")


target_link_libraries(${LIB_TARGET_NAME}
    PRIVATE
    OpenAssetIO::openassetio-core
	openassetio_grpc_grpc_proto
	${_REFLECTION}
	${_GRPC_GRPCPP}
	${_PROTOBUF_LIBPROTOBUF})


# -------------------------------------------------------------------------------------------------
# export.h

# Use CMake utility to generate the export header.
include(GenerateExportHeader)
generate_export_header(
    ${LIB_TARGET_NAME}
    EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/openassetio-grpc/export.h
)

install(
    FILES ${PROJECT_BINARY_DIR}/include/openassetio-grpc/export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/openassetio-grpc/
)

