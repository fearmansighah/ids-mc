#include <WiFi.h>
#include <PubSubClient.h>

// Replace the next variables with your SSID/Password combination
const char* ssid = "SPSETUP-0DB7";
const char* password = "happen8997chord";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "test.mosquitto.org";

WiFiClient esp32Client;
PubSubClient client(esp32Client);
long lastMsg = 0;


void setup() {
  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_server, 1883);

}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
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
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("esp32Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32-ids-ubco/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    
    // Temperature in Celsius
    double temperature = random(22,30);
    char tempString[8];
    dtostrf(temperature, 1, 2, tempString);
    Serial.print("Temperature: ");
    Serial.println(tempString);
    Serial.println("---------------------------------------");
    
    client.publish("esp32-ids-ubco/temperature", tempString);
  }
}
