# Core

## Install

```sh
sudo apt-get install -y \
    git \
    build-essential \
    cmake \
    libgl1-mesa-dev \
    libssl-dev \
    qtbase5-dev \
    qtdeclarative5-dev \
    qtwayland5 \
    qml-module-qtquick2 \
    qml-module-qtquick-controls2 \
    qtquickcontrols2-5-dev \
    qml-module-qtmultimedia \
    liblinphone-dev \
    libpaho-mqtt-dev \
    libpaho-mqttpp-dev
```

## Build

```sh
cmake -B build
make -C build
```

## Run

```sh
build/src/app/dieklingel-core
```
