#include <WiFi.h>
#include <PubSubClient.h>
#include <string.h>


#define LED 33

// WiFi
const char *ssid = "Vodafone2.4Ghz"; // Enter your WiFi name
const char *password = "Ne924YG4aA9HCZbY";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "Led";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
    //Set pin Mode
    pinMode(LED, OUTPUT);
    // Set software serial baud to 115200;
    Serial.begin(115200);
    // connecting to a WiFi network
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the WiFi network");
    //connecting to a mqtt broker
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    while (!client.connected()) {
        String client_id = "esp8266-client-";
        client_id += String(WiFi.macAddress());
        Serial.println("cliente id"+client_id);
        const char *clientId = client_id.c_str();
        Serial.println("Connecting to public emqx mqtt broker.....");
        if (client.connect(clientId, mqtt_username, mqtt_password)) {
            Serial.println("Public emqx mqtt broker connected");
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
    // publish and subscribe
    client.publish(topic, "hello emqx");
    client.subscribe(topic);
}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    String message;
    for (int i = 0; i < length; i++) {
        message = message + (char) payload[i];  // convert *byte to string
    }
    Serial.print(message);
    if(message.indexOf("on") != -1) { Serial.print("dentro"); digitalWrite(LED, HIGH); delay(1000);}   // LED on
    if(message.indexOf("off") != -1) { Serial.print("fuera");digitalWrite(LED, LOW); } // LED off
    Serial.println();
    Serial.println("-----------------------");
}

void loop() {
    client.loop();
}
