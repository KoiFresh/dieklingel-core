# dieklingel-core

![dieKlingel Logo und Text](https://raw.githubusercontent.com/KoiFresh/dieklingel-homepage/main/static/logo_2500x500_free.png)

dieKlingel is a project under the direction of
[Kai Mayer](https://github.com/KoiFresh) and
[Sven Schoch](https://www.instagram.com/sven.schoch.xx/). The purpose of this
project is to provide a open source doorbell with some cool functionality, like
connecting to video in realtime or integrate the doorbell into your smarthome
system like [fhem](https://fhem.de), to add custom actions when someone rings
the bell. In order to get this working we build the system on top of mqtt. The
project consits of multiple services, which shoudl act unindependet of each
other, but can use functions from other services. Each Service could be started
and stopped indiviually. This repository contains the core of the project. The
core can be installed on a raspberry pi and act as the main door unit. In order
to make the project useable there is another repository which contains source
code for dieKlingel App, a smaple application to interact with the interface the
core provides, for example video communication or show recent events on the core.

## Services

As mentioned before the core consists of multiple services, the services are
listet, below. For more information about a single service, checkout the README
in the service package root directory.

- **APN: App Push Notification**

   A service to send a push notification to the app, if someone rings the bell,
   the same thing could be achived by any many other systems like fhem, but in
   order to be able to use the core without a third party system we provide this
   simple service.

- **GUI: Graphical User Interface**

   This service provides the main interface for the outer door unit, where anybody
   can ring the bell or enter a passcode to open the door.

- **RTC: Real Time Connection**

   A service wich handles every part of the real time connection of the core. It
   also provides an interfaces to capture a snapshot of the current active video
   connection.

## Build the core

The whole core is wrapped into a single snap package, which can be easily
installed on any system, where [snap](https://snapcraft.io) is available. To
build the core snap package, simply install snapcraft and the run `snapcraft`
in the root of this project.

```bash
sudo snap install snapcraft --classic
cd dieklingel-core
snapcraft
```

After building the snap package there will be a a new fille with `.snap`
extension in the current folder, which than ca be installed and run.

## Install the core

We are running the core as a service to in the currently logged in user, this
feature is experimental currently to enable run:

```bash
snap set system experimental.user-daemons=true
```

The to install the package run:

```bash
snap install dieklingel-core${whatever the version and architecture is}.snap --devmode
```

And to get the snap to work as expected and use system ressources you have to
connect some plugs:

```bash
snap connect dieklingel-core:home
snap connect dieklingel-core:network
snap connect dieklingel-core:camera
snap connect dieklingel-core:audio-playback
snap connect dieklingel-core:audio-record
snap connect dieklingel-core:wayland
snap connect dieklingel-core:x11
```

## Run the core

- 🚀 start a service:

   **APN:**

   ```bash
   systemctl --user start snap.dieklingel-core.apn.service
   # or
   snap run dieklingel-core.apn
   ```

   **GUI:**

   ```bash
   systemctl --user start snap.dieklingel-core.gui.service
   # or
   snap run dieklingel-core.gui
   ```

   **RTC:**

   ```bash
   systemctl --user start snap.dieklingel-core.rtc.service
   # or
   snap run dieklingel-core.rtc
   ```

- ✋ stop a service:

   **APN:**

   ```bash
   systemctl --user stop snap.dieklingel-core.apn.service
   # or
   snap stop dieklingel-core.apn
   ```

   **GUI:**

   ```bash
   systemctl --user stop snap.dieklingel-core.gui.service
   # or
   snap stop dieklingel-core.gui
   ```

   **RTC:**

   ```bash
   systemctl --user stop snap.dieklingel-core.rtc.service
   # or
   snap stop dieklingel-core.rtc
   ```

- 🚗 enable autostart (enabled by default):

   **APN:**

   ```bash
   systemctl --user enable snap.dieklingel-core.apn.service
   ```

   **GUI:**

   ```bash
   systemctl --user enable snap.dieklingel-core.gui.service
   ```

   **RTC:**

   ```bash
   systemctl --user enable snap.dieklingel-core.rtc.service
   ```

- 🚦 disable autostart:

   **APN:**

   ```bash
   systemctl --user disable snap.dieklingel-core.apn.service
   ```

   **GUI:**

   ```bash
   systemctl --user disable snap.dieklingel-core.gui.service
   ```

   **RTC:**

   ```bash
   systemctl --user disable snap.dieklingel-core.rtc.service
   ```

- 🔄 restart a service:

   **APN:**

   ```bash
   systemctl --user restart snap.dieklingel-core.apn.service
   ```

   **GUI:**

   ```bash
   systemctl --user restart snap.dieklingel-core.gui.service
   ```

   **RTC:**

   ```bash
   systemctl --user restart snap.dieklingel-core.rtc.service
   ```
