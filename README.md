# 🔔 dieKlingel Core

Discover a project aimed at crafting a lightweight, feature-packed doorbell.
Enjoy video calling and remote door unlocking, all at a fraction of the usual
cost.

## 📦 Technologies

- [Linphone-SDK](https://gitlab.linphone.org/BC/public/linphone-sdk)
- [Paho MQTT](https://github.com/eclipse/paho.mqtt.cpp)
- [QT Framework](https://doc.qt.io/qt-5/)
- [CMake](https://cmake.org)

## 🦄 Features

dieKlingel is designed to run on a small computer like a  Raspberry Pi. It has
intergrations for different types of use-cases. We try to keep each use case as
easy to configure as possible.

### 📺 Kiosk

At the hearth of dieKlingel runs a kiosk application which is build with QML
which makes it possible to be extended as needed. The kiosk application is
desinged to run on small display and give the user the ability to ring the
bell, turn on the light or unlock the doo via a passcode on the screen.

### 📞 SIP

You need mediastreams? You get mediastreams! The SIP intergration allows it, to
ring your phone, when some action is going on infront of your door. With the
power of [Flexisip](https://www.linphone.org/technical-corner/flexisip) and the
[Linphone APP](https://www.linphone.org) you'll get notifications on your phone
even, if you aren't at home.

### 🕸️ MQTT

How would it be easier to intergrate dieKlingel in your smarthome without
coupling it to a decent provider? The mqtt intergration could be connected to
an mqtt broker, and publish events on topics, so that other smart devices could
react on these. Also states like "online" and "offline" could be gathered via
mqtt.

### 🕹️ GPIO

Do you prefer a physical button instead of the onscreen button? Don't worry,
use the gpio pins of your Raspberry Pi as input to react on a button press. An
integration which gets extremle powerfull in combination with the Kiosk, just
use QML to script the actions on an input event.

## 🚦 Run the Project

To be able to run the project you have to build it. Start by cloning this repository.

### 🪜 Install dependencies

Install dependencies which are required to build and/or run the project.

```sh
# Init the taskfile
bash taskfile init

# Buildtime dependencies
sudo ./taskfile deps:build

# Runtime dependencies
sudo ./taskfile deps:run

# Development dependencies
sudo ./taskfile deps:dev

# Or for development, build and runtime
sudo ./taskfile deps:all
```

### 🏗️ Build the core

```sh
./taskfile build

# Or build a debian package archive
./taskfile package
```

### 🔧 Install the core

```sh
sudo ./taskfile install

# Or if you build a debian package archive
sudo apt-get install ./build/dieklingel-core_<VERSION>_<ARCH>.deb
```

### 🚂 Run the core

Now you should be able to run the core with its default configuration. Don't worry you won't see much, because all integrations are disabled by default.

```sh
dieklingel-core
```

Now its time to enable the integrations you would like to use. The configuration file is called `core.js` and the core will look in multiple locations and choose the first file wich will be found.

- `$(pwd)/core.js`
- `$HOME/.config/core.js`
- `$HOME/core.js`
- `/etc/dieklingel-core/core.js`
- `/usr/share/dieklingel-core/core.js`

To configure the core as needed refere to [core.js](/resources/core.js) and [core.d.ts](/core.d.ts).

### 🚅 Run as service

- 🚀 Start as user service
    
    ```sh
    systemctl --user start dieklingel-core
    ```

- ✋ Stop as user service
    
    ```sh
    systemctl --user stop dieklingel-core
    ```

- 🔄 Restart the service
    
    ```sh
    systemctl --user restart dieklingel-core
    ```

- 🚗 Run after login
    
    ```sh
    systemctl --user enable dieklingel-core
    ```
