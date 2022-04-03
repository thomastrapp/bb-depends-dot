#!/usr/bin/env bash

set -e
set -x

perror_exit() { echo "$1" >&2 ; exit 1 ; }

THREADS=2

deps=(
  "BBRD_BOOST_VERSION"
  "BBRD_BOOST_SHA256"
  "BBRD_BOOST_INSTALL_PATH"
)
for dep in "${deps[@]}" ; do
  [[ -z "${!dep}" ]] && perror_exit "missing env var '$dep'"
done

[[ -f "$BBRD_BOOST_INSTALL_PATH/include/boost/version.hpp" ]] \
  && perror_exit "boost is already installed in $BBRD_BOOST_INSTALL_PATH"

boost_dl_url="https://boostorg.jfrog.io/artifactory/main/release/${BBRD_BOOST_VERSION}/source/boost_${BBRD_BOOST_VERSION//./_}.tar.gz"

build_dir=$(mktemp -d)
echo $build_dir
cd "$build_dir"

curl -L -o boost.tar.gz "$boost_dl_url"

SHASUM=sha256sum
hash $SHASUM || SHASUM="shasum -a 256"

$SHASUM -c <(echo "$BBRD_BOOST_SHA256  boost.tar.gz")

tar zxf boost.tar.gz
cd */

export CC=gcc CXX=g++
hash gcc-8 && export CC=gcc-8 CXX=g++-8
hash gcc-9 && export CC=gcc-9 CXX=g++-9
hash gcc-10 && export CC=gcc-10 CXX=g++-10
hash gcc-11 && export CC=gcc-11 CXX=g++-11
$CXX --version

./bootstrap.sh --with-libraries=regex,program_options,graph
./b2 -d0 toolset=$CC -j$THREADS --prefix="$BBRD_BOOST_INSTALL_PATH" runtime-link=static variant=release link=static install

