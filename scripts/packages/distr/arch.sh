#!/bin/sh

packages_list="base-devel git m4 scons clang gcc protobuf boost \
               curl python3 graphviz aarch64-linux-gnu-gcc      \
               python-graphviz"

sudo pacman --noconfirm -Sy ${packages_list}
