# dieklingel-core

the core repository containing the main packages of the core to run dieklingel

# snap connections

We also use the run as user experimental flag, to enable use:
 
```bash
snap set system experimental.user-daemons=true
```

```bash
snap install dieklingel-core* --devmode
```

In order for the core to work properly, we need to specify some connections for the snap

```bash
snap connect dieklingel-core:home
snap connect dieklingel-core:network
snap connect dieklingel-core:camera
snap connect dieklingel-core:audio-playback
snap connect dieklingel-core:audio-record
snap connect dieklingel-core:wayland
snap connect dieklingel-core:x11
```

start and stop

```bash
systemctl --user start snap.dieklingel-core.gui.service
systemctl --user stop snap.dieklingel-core.gui.service

systemctl --user start snap.dieklingel-core.rtc.service
systemctl --user stop snap.dieklingel-core.rtc.service

systemctl --user start snap.dieklingel-core.apn.service
systemctl --user stop snap.dieklingel-core.apn.service
```
