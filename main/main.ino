#include "sensors.h"
#include "wifiTime.h"
#include "aws.h"
#include "httpPOST.h"

#include <math.h>

uint32_t messageCounter   = 1;

void publishMessage() {
  
  // Build JSON
  StaticJsonDocument<4096> doc;

  // Getting Data
  doc = get_sensorsData();

  // Giving timestamp and MessageID
  int64_t slaveTimestamp = (int64_t)millis() + startEpochOffset;
  doc["slaveTimestamp"] = slaveTimestamp;
  doc["messageId"]      = messageCounter;
  messageCounter++;

  // Sending Over HTTP
  String resultHTTP = sendHTTP(doc);
  Serial.println(resultHTTP);

  // Sending Over MQTT
  int resultMQTT = sendMQTT(doc);
  if (resultMQTT) {
    Serial.println("Message published successfully.");
  } else {
    Serial.println("Failed to publish message.");
  }

}

void setup() {
  Serial.begin(115200);

  connectSensors();
  connectWiFi();
  connectAWS();
}

void loop() {
  mqttClient.loop();
  publishMessage();
  delay(2000);
}