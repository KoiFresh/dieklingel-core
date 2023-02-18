# gui

A new Flutter project.

## Getting Started

This project is a starting point for a Flutter application.

A few resources to get you started if this is your first Flutter project:

- [Lab: Write your first Flutter app](https://docs.flutter.dev/get-started/codelab)
- [Cookbook: Useful Flutter samples](https://docs.flutter.dev/cookbook)

For help getting started with Flutter development, view the
[online documentation](https://docs.flutter.dev/), which offers tutorials,
samples, guidance on mobile development, and a full API reference.

## MQTT Channels:

- `$BASE_CHANNEL/io/display/state`

   This Channel is used to read and write the current state of the display. Possible values are
   `on` and `off`. If the value is `off` the display is showing the scrennsaver, else the default sign 
   page is shown. Any value written other than `on` or `off` will be ignored.

- `$BASE_CHANNEL/io/activity/state`

   This Channel is used to read and write the current activity state of the gui. Possible values are
   `active` and `inactive`. If there is any interaction on the display side, the state will set to active. After a 
   timeout of 30 seconds without interaction the st . Any value written other than `active` or `inactive` will be ignored.

- `$BASE_CHANNEL/io/action/sign/clicked`

- `$BASE_CHANNEL/io/action/unlock/passcode`

# Snapcraft

Make sure to run the build runner, before building the snap

```bash
flutter pub run build_runner build -d
```
