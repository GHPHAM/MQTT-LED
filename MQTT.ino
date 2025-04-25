#include <WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "WiFi NAME";         // Replace with your WiFi name
const char* password = "WiFI PASSWORD"; // Replace with your WiFi password

// MQTT Broker settings
const char* mqtt_server = "BROKER_IP";       // Replace with your broker IP
const int mqtt_port = 1883;                  // Default MQTT port
const char* mqtt_username = "";              // Optional: MQTT username if required
const char* mqtt_password = "";              // Optional: MQTT password if required
const char* clientID = "ESP32_Light";        // Client ID for MQTT connection
const char* topic_sub = "home/light";        // Topic to subscribe to for light control

// Light pin
const int lightPin = 2;  // Using built-in LED on many ESP32 boards, change if needed

// Initialize WiFi and MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

// Reconnect to MQTT broker when connection is lost
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // Attempt to connect
    if (mqtt_username[0] == '\0') {
      // Connect without credentials
      if (client.connect(clientID)) {
        Serial.println("connected");

        // Subscribe to light control topic
        client.subscribe(topic_sub);
        Serial.println("Subscribed to: " + String(topic_sub));
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        delay(5000);
      }
    } else {
      // Connect with credentials
      if (client.connect(clientID, mqtt_username, mqtt_password)) {
        Serial.println("connected");

        // Subscribe to light control topic
        client.subscribe(topic_sub);
        Serial.println("Subscribed to: " + String(topic_sub));
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        delay(5000);
      }
    }
  }
}

// Callback function when an MQTT message arrives
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");

  // Convert payload to string
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  // If message is "1" turn the light on
  if (message == "1") {
    digitalWrite(lightPin, HIGH);
    Serial.println("Light turned ON");
  }
  // If message is "0" turn the light off
  else if (message == "0") {
    digitalWrite(lightPin, LOW);
    Serial.println("Light turned OFF");
  }
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Set light pin as output
  pinMode(lightPin, OUTPUT);
  digitalWrite(lightPin, LOW); // Start with light off

  // Connect to WiFi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Set MQTT server and callback function
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  // If client is not connected, reconnect
  if (!client.connected()) {
    reconnect();
  }

  // Process MQTT messages
  client.loop();
}