#include "sensors.h"
#include "httpPOST.h"
#include "wifiTime.h"
#include "aws.h"


#include <math.h>

int LIMIT = 20;  //Limit of Messages
unsigned long TIMETORESENDMSGS =  60    *(1000);  //Millisecond
int ITERATIONS = 5;
int LOOPTIME = 2000; //Milliseconds


uint32_t messageCounter = 1;
int iteration = 1;
StaticJsonDocument<4096> doc;
unsigned long endMsgTime = 0;



void publishMessage() {
  
  // Build JSON
  //doc.clear();

  // Getting Data
  //doc = get_sensorsData();

  // Giving timestamp
  int64_t slaveTimestamp = (int64_t)millis() + startEpochOffset;
  doc["slaveTimestamp"] = slaveTimestamp;

  // Giving Message ID
  //doc["messageId"]      = messageCounter;
  

  // Sending Over HTTP
  // String resultHTTP = sendHTTP(doc);
  // Serial.println(resultHTTP);

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

  //connectSensors();
  connectWiFi();
  connectAWS();

  doc["User"] = "Basil Bhai";
  doc["Device"] = "ESP32";
  doc["Model"] = "WROOM-DA";
  doc["Messagae"] = "Altaf Bhai";
}

void loop() {
  //mqttClient.loop();

  if (messageCounter < LIMIT + 1) {


    Serial.printf("\nMessage Number %d sent\n", messageCounter);
    publishMessage();



    // Print Data Payload
    Serial.printf("PayloadSize(bytes): %u\n", sizeof(doc));

    // Printing Research Parameters
    Serial.printf("WifiStrength(dBm): %ld\n", getWifiStrength());


    messageCounter++;
    delay(LOOPTIME);
  }

  if (messageCounter == LIMIT+1){
    Serial.println();
    Serial.println();
    Serial.printf("Batch of %d is transfered, now going to iteration %d", LIMIT, iteration+1);
    messageCounter++;

    endMsgTime = millis();
  }

  if (millis() > endMsgTime + TIMETORESENDMSGS && messageCounter > LIMIT && iteration < ITERATIONS){
    messageCounter = 1;
    iteration++;
  }

  if (iteration > ITERATIONS){
    Serial.printf("Limit of %d iterations is completed, restart your device to start scan again", ITERATIONS);
  }
}