# Core

## Install

```sh
sudo apt-get install -y \
    git \
    build-essential \
    cmake \
    libgl1-mesa-dev \
    qt5base-dev \
    qtdeclarative5-dev \
    qtwayland5 \
    qml-module-qtquick2 \
    qml-module-qtquick-controls \
    liblinphone-dev
```

## Build

```sh
cmake -B build
make -C build
```

## Run

```sh
build/core
```
