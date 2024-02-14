#!/bin/sh

set -eou pipefail

fs_image_src="gs://dist.gem5.org/dist/v22-0/arm/disks"
fs_image_name="ubuntu-18.04-arm64-docker.img"
fs_image_name_bz2=${fs_image_name}".bz2"

script_path=$(dirname $(realpath $0))
fs_images_folder="fs_images"
fs_images_path=${script_path}/../../${fs_images_folder}

mkdir -p ${fs_images_path}
gsutil cp -r ${fs_image_src}/${fs_image_name_bz2} ${fs_images_path}

bzip2 -dfc ${fs_images_path}/${fs_image_name_bz2} > ${fs_images_path}/${fs_image_name}
