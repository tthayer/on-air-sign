#!/usr/bin/env bash
# This script monitors the input level status by calling an applescript command to poll the volume settings.
# If the input volume is over 0 it toggles the on-air endpoint and if it is 0 it toggles the off-air endpoint.

export on_air="http://on-air-esp32.lan/on-air"
export off_air="http://on-air-esp32.lan/off-air"

while true; do
    current_volume=$(osascript -e 'input volume of (get volume settings)')

    if [ $current_volume = "0" ]; then
        curl -q "$off_air" > /dev/null 2>&1
    else
        echo $current_volume > current_volume.txt
        curl -q "$on_air" > /dev/null 2>&1
    fi
    unset current_volume
    sleep 0.25
done
