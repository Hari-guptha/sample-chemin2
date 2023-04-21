#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <WebSocketsClient.h>
#include <DHT.h>  // include the DHT library

#define SSID "Titan"
#define PASSWORD "12346789"
#define SERVER "167.172.144.39"
#define DHTPIN 5 // Changed from D5 to D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

WebSocketsClient webSocket;

void setupWiFi() {
  Serial.println("\nConnecting...");

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nConnected : ");
  Serial.println(WiFi.localIP());
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  setupWiFi();

  webSocket.begin(SERVER, 3000, "/socket.io/?EIO=4");

  dht.begin(); // initialize the DHT sensor
}

void loop() {
  webSocket.loop();

  // read the temperature and humidity data from the sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (!isnan(temperature) && !isnan(humidity)) {
    // create a JSON string with the sensor data
    String data = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + "}";
    // send the sensor data to the server
    webSocket.sendTXT(data);
    Serial.println("Sent data: " + data);
  }

  delay(5000); // wait 5 seconds before sending the next sensor data
}
