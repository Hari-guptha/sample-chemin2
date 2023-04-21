#include <DHT.h>
#include <ESP8266WiFi.h>
#include <SocketIOClient.h>

#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Titan";
const char* password = "12346789";

const char* serverName = "167.172.144.39";
const int serverPort = 4006;
SocketIOClient socket;

void setup() {
  Serial.begin(9600);

  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi.");

  socket.begin(serverName, serverPort);
  socket.on("connect", []() {
    Serial.println("Connected to server.");
  });
  socket.on("disconnect", []() {
    Serial.println("Disconnected from server.");
  });
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  if (!isnan(temperature) && !isnan(humidity)) {
    String data = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + "}";
    socket.emit("sensorData", data);
    Serial.println("Sent data: " + data);
  }

  socket.loop();
  delay(10000);
}
