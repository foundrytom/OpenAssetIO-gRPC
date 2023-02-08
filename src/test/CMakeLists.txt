# SPDX-License-Identifier: Apache-2.0 Copyright 2013-2023 The Foundry
# Visionmongers Ltd

set(TESTHOST_TARGET_NAME openassetio-grpc-testhost)

set(SRC main.cpp)

add_executable(${TESTHOST_TARGET_NAME} ${SRC})

openassetio_grpc_set_default_target_properties(${TESTHOST_TARGET_NAME})

target_link_libraries(${TESTHOST_TARGET_NAME}
    openassetio-grpc
    OpenAssetIO::openassetio-core)

install(TARGETS ${TESTHOST_TARGET_NAME} DESTINATION .)
