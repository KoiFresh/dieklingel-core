# Core

## Install

```sh
sudo apt-get install -y git build-essential cmake libgl1-mesa-dev qt6-base-dev qt6-declarative-dev qt6-wayland liblinphone-dev
```

## Build

```sh
rm -rf build
mkdir build
cd build
qmake6 ../
make
cd ../
```

## Run

```sh
build/bin/core
```
