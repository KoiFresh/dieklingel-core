# dieklingel-core

the core repository containing the main packages of the core to run dieklingel

# snap connections

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