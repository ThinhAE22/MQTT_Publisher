#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
const char* mqtt_server = "test.mosquitto.org";

int buttonPin = D0;

// Global variable to store the received state
String state = "";

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password, 6);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}


void reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect("arduinoClient")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    } 
  }
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  
  Serial.print("Message: ");
  state = ""; // Clear the previous state
  for (int i = 0; i < length; i++) {
      state += (char)payload[i];  // Convert payload to String
  }
  Serial.println(state);
  Serial.println("-----------------------");
}


void setup()
{
  Serial.begin(9600);
  delay(2000);
  Serial.println("initWifi");
  initWiFi();
  Serial.println("mqtt setServer");
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(callback);
  reconnect();
  mqttClient.subscribe("Seamk_testi");
  mqttClient.publish("Seamk_testi","off");
  delay(1500);
  pinMode(buttonPin,INPUT);
}

void loop()
{

  if (!mqttClient.connected()) {
    reconnect();
  }
  if (digitalRead(buttonPin)) {
    mqttClient.publish("Seamk_testi","on");
  } else {
    mqttClient.publish("Seamk_testi","off");
  }
  mqttClient.loop();
  //Serial.println(digitalRead(buttonPin));
  delay(100);
}