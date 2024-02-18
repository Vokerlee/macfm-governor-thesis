#!/bin/sh

set -eou pipefail

# Script config

script_path=$(dirname $(realpath $0))
repo_dir=$(realpath ${script_path}/..)
gem5_dir=gem5

# Script start

cd ${repo_dir}/${gem5_dir}
make -C util/term
sudo install -o root -m 555 util/term/m5term /usr/local/bin
source ~/.bashrc
