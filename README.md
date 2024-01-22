# Core

## Install

```sh
sudo apt-get install -y git build-essential qt6-base-dev liblinphone-dev
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
