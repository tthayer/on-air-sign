# On-air Indicator

You're muted, dude!

## Requirements

* ESP32 hardware of some kind. This project was written for an M5Stack Atom development platform.
* VSCode + Platform.io installed and configured
* A computer running MacOS
* skhd (typically installed via homebrew)

## Setup
1. Copy `src/config.h.sample` to `src/config.h` and edit the WiFi connection information there to match your network.
2. Build the project and upload it to your device.
3. Edit `toggle_input_level.scpt` to set the location of `current_volume.txt` to the project location on your disk.
4. Run `./background_input_monitor.sh &` to background it. Or run it without the ampersand if you want it to run in the foreground.
5. Edit your `~/.skhdrc` file to include a keyboard shortcut to run the input-level toggle as described below and run `brew services restart skhd` to reload the configuration.
6. Enjoy your glowing LED.

## How it works

The ESP32 is running a web server with two endpoints and a default networking configuration
that will register a hostname on your network as `on-air-esp32.lan`. Two endpoints are available
for updating your mute status:
    * `/on-air`: Green light, you're unmuted!
    * `/off-air`: Red light, you're muted!

The `background_input_monitor.sh` script continuously monitors the volume level by polling the
MacOS system preferences via applescript. It saves the volume level to a file if that level is over '0'.
This script needs to be running in the background before you run the toggle. If the `current_volume.txt` file
is missing or the location is incorrectly defined in the `toggle_input_level.scpt` script, that script will fail
to run.

The `toggle_input_level.scpt` script is an applescript that toggles the input volume level by reading the
contents of the file created by `background_input_monitor.sh`.

The `toggle_input_level.scpt` script is triggered by a keyboard shortcut defined in your skhd preferences.
Example using the left-hand modifiers + the 'm' key:
`lshift + lalt + lctrl + lcmd - m : osascript /Users/tony/projects/on-air-sign/toggle_input_level.scpt`

This will globally mute your microphone and update the LED on your microcontroller, independent of the
application currently using your mic. The downside is your applications will no longer indicate your mute status
but the upside is if this breaks horribly you won't have to talk in meetings anymore.
