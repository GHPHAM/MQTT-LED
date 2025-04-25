toggle-mqtt-light.sh#!/bin/bash
# File: /usr/local/bin/toggle-mqtt-light.sh

# Path to mosquitto_pub command
MOSQUITTO_PUB="/usr/bin/mosquitto_pub"

# MQTT broker settings
BROKER_IP="BROKER_IP" # Lol not going to doxx my IP
TOPIC="home/light"

# Read current state from state file
STATE_FILE="/tmp/mqtt_light_state"

if [ ! -f "$STATE_FILE" ]; then
    # If state file doesn't exist, create it with initial state of 0 (off)
    echo "0" > "$STATE_FILE"
fi

# Read current state
CURRENT_STATE=$(cat "$STATE_FILE")

# Toggle state
if [ "$CURRENT_STATE" = "0" ]; then
    NEW_STATE="1"
    echo "Turning light ON"
else
    NEW_STATE="0"
    echo "Turning light OFF"
fi

# Save new state
echo "$NEW_STATE" > "$STATE_FILE"

# Send MQTT message
$MOSQUITTO_PUB -h "$BROKER_IP" -t "$TOPIC" -m "$NEW_STATE"

# Log the action
logger -t mqtt-light "Light toggled to state: $NEW_STATE"