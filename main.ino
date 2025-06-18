#include "sensors.h"
#include "wifiTime.h"
#include "aws.h"

#include <math.h>

uint32_t messageCounter   = 1;

void publishMessage() {
  
  // Build JSON
  StaticJsonDocument<4096> doc;
  doc = get_sensorsData();

  int64_t slaveTimestamp = (int64_t)millis() + startEpochOffset;
  doc["slaveTimestamp"] = slaveTimestamp;
  doc["messageId"]      = messageCounter;
  messageCounter++;

  int result = sendMQTT(doc);
  if (result) {
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