# OpenAssetIO gRPC

## Goal

Provide a low-friction way for hosts to integrate OpenAssetIO such that
side-effects on its runtime are minimised.

> **Note:**
> This is a proof-of-concept project that aims to explore and understand
> the potential benefits, and costs of using gRPC to achieve this goal.
> The code is by no means well robust or well tested.

Bonus: The included `.proto` allows a gRPC implementation of the
OpenAssetIO `ManagerInterface` in C#, Dart, Go, Java, Kotlin, Node,
Objective-C, PHP, Ruby without explicit support in the core language
layers.

## Background

The use of Python in common deployment scenarios can cause assorted
problems. In applications where the UI is implemented using some Python
based framework, the dreaded GIL means that any other work done in
Python can ultimately stall the UI event loop. For example, a large
number of calls to a python Manager implementation from a background
work thread.

## Approach

Employ gRPC to move OpenAssetIO functionality out-of-process. This will
add some per-call overhead, but in theory provides the following
benefits:

- Decouples the Host runtime and its dependencies from OpenAssetIO,
  this could include compiler or python versions.
- Manager caching could be leveraged across multiple processes.
- Could be used as a bridge into other languages.
- Multiple out-of-process servers could be pooled.

There are some downsides to this idea:

- There is most likely to be a non-trivial performance overhead
  compared to an in-process implementation.
- The manager plugin no longer has direct access to the host
  environment.

### Architecture

The project consists of an alternate `ManagerImplementationFactory`
(`GRPCManagerImplementationFactrory`). When used by a host, this returns
a custom `ManagerInterface` shim, that proxies all API calls to the
configured gRPC endpoint - assuming it implements the service defined in
[openassetio.proto](./src/openassetio.proto).

There are three components to this project:

- The `openassetio-grpc` library, that provides an SDK for OpenAssetIO
  Hosts.
- The `openassetio-grpc-server` binary (or Python module) that
  instantiates managers to service the bridged API requests.
- The `openassetio-grpc-test` high-level integration test suite.
- The `openassetio-grpc-perftest` benchmark test that compares direct
  and gRPC bridged python manager interactions.

The Python server implementation can be run in 'out-of-process' mode, to
instantiate manager instanced on demand, or in 'shared' mode, where an
instance of the `$OPENASSETIO_DEFAULT_CONFIG` manager is available to an
arbitrary number of remote clients via the `shared` handle.

### Building

The code currently requires gRPC and OpenAssetIO to be installed in
such a way as to be visible to CMake. The included `conanfile.txt` can
be used along with `cmake --toolchain conan_paths.cmake` if so desired.

Assuming your working directory is in the root of this repository.

```bash
cmake -S . build -DCMAKE_BUILD_TYPE=Release
cmake --build build && cmake --install build
```

This will populate `build/dist` with the project components.

> **Warning:**
> The current build infrastructure is somewhat work-in-progress.
> Components are built as shared libraries to avoid having to juggle
> the gPRC global state at this point.

### Swapping out the factory

All you need to do to use the bridge in a host is to use the gRPC
manager impl factory in place of your existing one (`auto` used for
brevity). NB. Port configuration is currently hard coded.

```cpp
auto logger = ...
auto implFactory = GRPCManagerImplementationFactory::make("0.0.0.0:51234", logger);
auto hostInterface = ...
auto mgrFactory = ManagerFactory::make(hostInterface, implFactory, logger);
```

## Testing steps

> **Note:**
> In its present state, the testing process is somewhat manual. A
> production version of this code would probably take care of managing a
> server process for you.

The following assumes you are at the root of this repository and have
successfully built the various components.

### Server

The preferred approach is to use the Python server implementation, as
this greatly simplifies the build and runtime environment setup:

```shell
python -m pip install ./src/server-python
python -m openassetio_grpc_server
```

The `openassetio-grpc-server` receives requests over gRPC and wrangles
its own `ManagerFactory` to initialize host-requested managers,
forwarding subsequent requests to the appropriate instance. Each server
can be multi-tenanted, managing any number of discrete managers.

If you wish to provision a shared instance of the manager defined by
`$OPENASSETIO_DEFAULT_CONFIG`, then append the `-d` flag. This also
initializes the default manager, and routes requests to the `shared`
handle to it. A host requesting the factory to instantiate the
`org.openassetio.gRPC.manager` manager, initializing with a suitable
`channel` addressing the server will then be able to access this
instance, locally or across a network. This has little practical use
outside of the ad-hoc provision of multi-client testing when using a
simple, single-tenanted manager such as BAL.

There is also a less featured/experimental C++ Server available, with
limited API support.


```shell
# Required for the embedded interpreter
export PYTHONHOME=/path/to/python/root/used/for/build
# Ensure OpenAssetIO + bridge libs are available
export LD_LIBRARY_PATH=./build/dist/lib:/path/to/OpenAssetIO/libs
export PYTHONPATH=/path/to/OpenAssetIO/site-packages:$PYTHONPATH
# Keep an eye on things
export OPENASSETIO_LOGGING_SEVERITY=0
```

N.B If `importlib_metadata` is missing from the Python distribution you
used to build, you'll need to manually set `$OPENASSETIO_PLUGIN_PATH` if
you are relying on entry point discovery.

```bash
export OPENASSETIO_PLUGIN_PATH=/path/to/OpenAssetio-Manager-BAL/plugin
```

You should now be able to run the server:

```bash
$ ./build/dist/openassetio-grpc-server
       info: Server listening on 0.0.0.0:51234
```

### Test suite

The `openassetio-grpc-test` is a simple C++ test suite that uses
the gRPC bridge to resolve a string through the default configured
manager (see [here]("https://openassetio.github.io/OpenAssetIO/classopenassetio_1_1v1_1_1host_api_1_1_manager_factory.html#a8b6c44543faebcb1b441bbf63c064c76)
for more info).

Similar to the server, it currently needs a few vars setting. In a new
shell:

```bash
# Ensure OpenAssetIO and bridge libs are available
export LD_LIBRARY_PATH=./build/dist/lib:/path/to/OpenAssetIO/libs
# Ensure we can see the program output
export OPENASSETIO_LOGGING_SEVERITY=0
```

As the test host uses the default manager mechanism, so we need
to use the supplied config.

```bash
export OPENASSETIO_DEFAULT_CONFIG=./openassetio_conf.toml
```

This requires BAL, if you're using the python server, you can use Pip to
install it:

```shell
python -m pip install openassetio-manager-bal
```

Running the tests should hopefully then pass.

```bash
$ ./build/dist/openassetio-grpc-test
===============================================================================
All tests passed (7 assertions in 1 test case)
```

## Notes and limitations

- gRPC adds approximately an 6-7x overhead compared to local python per
  call. The included `openassetio-grpc-perftest` binary illustrates
  this.
  - Increasing `OPENASSETIO_LOGGING_SEVERITY` >= `3` on the server side
    reduces overhead as writing to the terminal is synchronous.
  - Adding support for `kEntityReferenceMatchPrefix` to `Manager.cpp`
    removes the bridged `isEntityReferenceString` calls gets this down
    to 5x.
  - Release builds are notable faster than debug.

- There is currently very little in the way of error handling, the
  unhappy path will most likely cause either the test host or server to
  fall over.

- No conscious effort has been made to optimize the implementation or
  proto.

## Todo

- Complete `ManagerInterface` methods.
- Return logging to the host.
- Manager servers in the impl factory.
- Proper error handling.
- Proper test coverage.
- Support manager state objects.
- Allow the shim interface to be user-configured without the custom
  factory (needs the C++ plugin system, so the it can be registered as a
  normal manager implementation without the host needing to adopt the
  GRPC factory).
