#include "sensors.h"
#include "httpPOST.h"
#include "wifiTime.h"
#include "aws.h"


#include <math.h>
int limit = 200;

uint32_t messageCounter = 1;

StaticJsonDocument<4096> doc;

String keys = "mm";



void publishMessage() {
  
  // Build JSON
  //doc.clear();

  // Getting Data
  //doc = get_sensorsData();

  // Giving timestamp
  // int64_t slaveTimestamp = (int64_t)millis() + startEpochOffset;
  // doc["slaveTimestamp"] = slaveTimestamp;

  // Giving Message ID
  //doc["messageId"]      = messageCounter;
  

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

  doc["m"] = "a";
}

void loop() {
  mqttClient.loop();
  if (messageCounter < limit + 1) {



    if (messageCounter%20 == 0.0){
      // Grow string up to a limit
      String m = doc["m"] | "";
      if (m.length() < 1024) {
        doc["m"] = m + "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
      }
    }

    if ((messageCounter+10)%20 == 0.0){
      // Grow string up to a limit
      doc[keys] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
      keys = keys + "m";
    }

    Serial.printf("\nMessage Number %d sent\n", messageCounter)
    publishMessage();

    // Printing Research Parameters
    Serial.printf("WifiStrength(dBm): %ld\n", getWifiStrength());

    float downSpeed = getDownloadSpeedKbps();
    Serial.printf("DownloadSpeed(kbps): %.2f\n", downSpeed);


    messageCounter++;
    delay(2000);
  }

  if (messageCounter == limit+1){
    Serial.println();
    Serial.println();
    Serial.printf("Limit of %d completed, restart your device to start scan again", limit);
    messageCounter++;
  }
}