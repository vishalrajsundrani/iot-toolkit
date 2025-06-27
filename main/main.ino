//Check for MQTT

#include "requirements.h" 
#include "secret.h"
#include "wifiTime.h"
#include "aws.h"  



int LIMIT = 10;  // Number of messages per iteration
unsigned long TIMETORESENDMSGS = 60 * 1000;  // 1 minute wait between iterations
int ITERATIONS = 5;  // Total iterations
int LOOPTIME = 1000;  // 1 second delay between messages

uint32_t messageCounter = 1;
int iteration = 1;
StaticJsonDocument<DOCSIZE> doc;
unsigned long endMsgTime = 0;

void publishMessage() {
  doc.clear();

  // Static fields
  doc["User"]    = "Basil Bhai";
  doc["Device"]  = "ESP32";
  doc["Model"]   = "WROOM-DA";
  doc["Message"] = "Altaf Bhai";

  // Timestamp
  int64_t slaveTimestamp = (int64_t)millis() + startEpochOffset;
  doc["slaveTimestamp"] = slaveTimestamp;

  // Send over MQTT
  bool resultMQTT = sendMQTT(doc);
  Serial.println("=== MQTT Result ===");
  if (resultMQTT){
    Serial.println("MQTT Message Sent Successfully");
  } else {
    Serial.println("Error Sending MQTT Message");
  }
  
}

void setup() {
  Serial.begin(115200);

  connectWiFi();
  connectAWS();
}

void loop() {
  if (messageCounter <= LIMIT) {
    Serial.printf("\nMessage Number %d sent\n", messageCounter);  // <== Restore this
    publishMessage();

    // Print Payload info
    Serial.printf("PayloadSize(bytes): %u\n", measureJson(doc));
    Serial.printf("WifiStrength(dBm): %ld\n", getWifiStrength());

    messageCounter++;
    delay(LOOPTIME);
}


  if (messageCounter == LIMIT + 1) {
    Serial.println();
    Serial.printf("Batch of %d is transferred. Iteration %d is completed, now going to iteration %d\n", LIMIT, iteration, iteration + 1);
    messageCounter++;  // Prevent re-entering this block
    endMsgTime = millis();
  }

  if (millis() > endMsgTime + TIMETORESENDMSGS && messageCounter > LIMIT && iteration < ITERATIONS) {
    messageCounter = 1;
    iteration++;
  }

  if (iteration >= ITERATIONS) {
    Serial.printf("Limit of %d iterations completed. Restart device to start scan again.\n", ITERATIONS);
    while (true);  // Stop the loop
  }
}

