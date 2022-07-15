# Serialization using introspection example

## Overview

Both serialization and introspection are implemented in the `intro` library.

## Usage

Default usage:
```
bazel run //example/http:server
```

With a custom compiler
```
CC=<compiler-binary> bazel run //example/http:server
```
