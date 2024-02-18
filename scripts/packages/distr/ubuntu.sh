#!/bin/sh

packages_list="build-essential git m4 scons zlib1g zlib1g-dev                \
    libprotobuf-dev protobuf-compiler libprotoc-dev libgoogle-perftools-dev  \
    python3-dev libboost-all-dev pkg-config python3-tk gcc-aarch64-linux-gnu \
    python-pydot python-pydot-ng graphviz"

sudo apt update
sudo apt -y install ${packages_list}
