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

## Run the core

1. Install Ubuntu Frame:

   ```bash
   sudo snap install ubuntu-frame
   ```

2. Install Pulseaudio Snap:

   ```bash
   sudo snap install pulseaudio
   ```

3. Install the dieKlingel Core:

   ```bash
   sudo snap install dieklingel-core
   ```

4. Connect Plugs and Slots:

   ```bash
   /snap/dieklingel-core/current/bin/setup.sh
   ```

   or

   ```bash
   snap connect dieklingel-core:opengl
   snap connect dieklingel-core:wayland
   snap connect dieklingel-core:network
   snap connect dieklingel-core:camera
   snap connect dieklingel-core:audio-playback
   snap connect dieklingel-core:audio-record
   ```

5. Autostart Ubuntu-Frame and Core

   ```bash
   sudo snap set ubuntu-frame daemon=true
   sudo snap set dieklingel-core daemon=true
   ```
