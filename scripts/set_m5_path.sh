#!/bin/sh

set -eou pipefail

# Script config

script_path=$(dirname $(realpath $0))
repo_dir=$(realpath ${script_path}/..)

# Script start

sed '/M5_PATH/d' ~/.bashrc > ~/.bashrc_m5
mv ~/.bashrc_m5 ~/.bashrc
echo "export M5_PATH=${repo_dir}" >> ~/.bashrc
source ~/.bashrc
