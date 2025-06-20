#include "sensors.h"
#include "wifiTime.h"
#include "aws.h"
#include "httpPOST.h"

#include <math.h>
int limit = 40;

uint32_t messageCounter = 1;


void publishMessage() {
  
  // Build JSON
  StaticJsonDocument<4096> doc;

  // Getting Data
  doc = get_sensorsData();

  // Giving timestamp
  int64_t slaveTimestamp = (int64_t)millis() + startEpochOffset;
  doc["slaveTimestamp"] = slaveTimestamp;

  // Giving Message ID
  doc["messageId"]      = messageCounter;
  messageCounter++;

  // Giving WiFi Strength
  doc["WifiStrength(dBm)"] = getWifiStrength();

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
  if (messageCounter < limit+1){
    publishMessage();
    delay(2000);
  }
  if (messageCounter == limit+1){
    Serial.println();
    Serial.println();
    Serial.println("Limit of 20 completed, restart your device to start scan again");
    messageCounter++;
  }
}
