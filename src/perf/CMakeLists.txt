# SPDX-License-Identifier: Apache-2.0
# Copyright 2013-2023 The Foundry Visionmongers Ltd

set(PERFTEST_TARGET_NAME openassetio-grpc-perftest)

set(SRC main.cpp)

# Python

find_package(Python COMPONENTS Interpreter Development.Embed REQUIRED)

if (NOT ${Python_VERSION_MAJOR} EQUAL ${OpenAssetIO_Python_VERSION_MAJOR} OR
    NOT ${Python_VERSION_MINOR} EQUAL ${OpenAssetIO_Python_VERSION_MINOR})
    message(
        FATAL_ERROR
        "Found Python '${Python_VERSION}' but OpenAssetIO was built against"
        " '${OpenAssetIO_Python_VERSION}'"
    )
endif ()

#


add_executable(${PERFTEST_TARGET_NAME} ${SRC})

# If libpython is linked in as a static library, then we must export
# symbols for dynamically loaded Python extension modules to use.
set_target_properties(${PERFTEST_TARGET_NAME} PROPERTIES ENABLE_EXPORTS ON)

openassetio_grpc_set_default_target_properties(${PERFTEST_TARGET_NAME})

target_link_libraries(${PERFTEST_TARGET_NAME}
    openassetio-grpc
    OpenAssetIO::openassetio-core
    OpenAssetIO::openassetio-python-bridge
    Python::Python)

install(TARGETS ${PERFTEST_TARGET_NAME} DESTINATION .)
