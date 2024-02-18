# Full description of required actions to launch linux with Gem5

## Pre-build

```shell
git submodule update
```

## Linux kernel v4.15 build

```shell
cd linux
git apply -3 ../linux_patches/*.patch
make KCONFIG_CONFIG=../linux_configs/gem5_arm64_config ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -j16
cd ..
```

## Gem5 build

Build for `arm`/`arm64` architectures:

```shell
cd gem5
git apply -3 ../gem5_patches/*.patch
scons build/ARM/gem5.opt -j16
cd ..
```

Also we should installl `m5term` for accessing loaded kernel via terminal:
```shell
bash scripts/install_m5term.sh
```

## Launch linux with Gem5

First we should set `M5_PATH` variable, gem5 config scripts will use it to found
binaries folder with bootloaders.
```shell
bash scripts/set_m5_path.sh
```

After that we must build bootloader/s for `arm64` architecture:
```shell
bash scripts/build_gem5_bootloader.sh
```

Disk image can be downloaded by the following script:
```shell
bash scripts/download_disk_image.sh
```

Finally, launch kernel with command:
**To be updated**:
```shell
cd gem5
./build/ARM/gem5.opt configs/example/arm/starter_fs.py \
    --kernel=../linux/vmlinux \
    --disk-image=../fs_images/ubuntu-18.04-arm64-docker.img
    # Bootloader will be found at binaries folder by script
```

How to connect to loaded kernel:
```shell
# Usually port_name is 3456
m5term localhost [port name]
```
