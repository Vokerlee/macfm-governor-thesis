#!/bin/sh

set -eou pipefail

# Script config

script_path=$(dirname $(realpath $0))
repo_dir=$(realpath ${script_path}/..)
gem5_dir=gem5
binaries_dir=binaries

# Script start

mkdir -p ${repo_dir}/${binaries_dir}
cd ${repo_dir}/${gem5_dir}

make -C system/arm/bootloader/arm64
cp -r system/arm/bootloader/arm64/boot.arm64 ../${binaries_dir}

# Just copy arm64 bootloader to imitate having arm bootloader
cp -r ../${binaries_dir}/boot.arm64 ../${binaries_dir}/boot.arm
