#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "20 Laboratory";
const char* password = "mg;o9uhld^Ux";
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();
  controlPins((char)payload[0] == '0', (char)payload[0]);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("20scoopsClawMachine");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.subscribe("20scoopsClawMachine");
}

void loop() {
  if (!client.connected())reconnect();
  client.loop();
}

void controlPins(bool flag, char pin) {
  if (!flag) {
    if (pin == '1') digitalWrite(0, HIGH);
    else if (pin == '2') digitalWrite(2, HIGH);
    else if (pin == '3') digitalWrite(15, HIGH);
    else if (pin == '4') digitalWrite(4, HIGH);
    else if (pin == '5') digitalWrite(5, HIGH);
  } else {
    digitalWrite(0, LOW);
    digitalWrite(2, LOW);
    digitalWrite(15, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  }
}
