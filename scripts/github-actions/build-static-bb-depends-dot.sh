#!/usr/bin/env bash

set -e

BBRDD="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )/../../"
LIBBBRDD="$BBRDD/build"

cd "$BBRDD/build"
cmake \
  -DCMAKE_POLICY_DEFAULT_CMP0074=NEW \
  -DBOOST_ROOT="$BBRD_BOOST_INSTALL_PATH" \
  -DBoost_USE_STATIC_LIBS=On \
  -DBoost_USE_STATIC_RUNTIME=On \
  -DCMAKE_EXE_LINKER_FLAGS=" -static-libgcc -static-libstdc++ " \
  ..
make $CMAKE_MAKE_FLAGS
make install

bb-depends-dot --version
ldd $(which bb-depends-dot)
