# OpenAssetIO gRPC

> **Note:**
> This is a proof-or-concept project

## Goal

Provide a low-friction way for hosts integrate OpenAssetIO such that
side-effects on it's runtime are minimised.

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

- Provide a `GRPCManagerImplementationFactrory` that can be used as a
  drop-in replacement to the
  `PythonPluginSystemManagerImplementationFactory` that provides gRPC
  abstracted interface instances.
- Expose a `GRPCManagerInterface` that can be used directly with a
  known channel to an existing server via its settings.
