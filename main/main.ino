
#include "requirements.h" 
#include "secret.h"
#include "wifiTime.h"

#include "aws.h"
#include "httpPOST.h"
#include "sensors.h"


uint32_t messageCounter = 1;
int iteration = 1;
StaticJsonDocument<DOCSIZE> doc;
unsigned long endMsgTime = 0;

String m = "mm";


void publishMessage() {
  if (!INCLUDE_DUMMY_INCREASING_DATA) doc.clear();

  if (INCLUDE_SENSOR_DATA) doc = get_sensorsData();

  // Static fields
  if (INCLUDE_DUMMY_STATIONARY_DATA){
    doc["User"]    = "Basil Bhai";
    doc["Device"]  = "ESP32";
    doc["Model"]   = "WROOM-DA";
    doc["Message"] = "Tariq Mumtaz is our supervisor";
  }

  // Timestamp
  if (INLCUDE_MANDATORY_DATA){
    int64_t slaveTimestamp = (int64_t)millis() + startEpochOffset;
    doc["slaveTimestamp"] = slaveTimestamp;
  }

  // Send over MQTT
  if (SEND_MQTT_DATA){
    bool resultMQTT = sendMQTT(doc);
    Serial.println("=== MQTT Result ===");
    if (resultMQTT){
      Serial.println("MQTT Message Sent Successfully");
    } else {
      Serial.println("Error Sending MQTT Message");
    }
  }

  // Send over HTTP
  if (SEND_HTTP_DATA){
    bool resultHTTP = sendHTTP(doc);
    Serial.println("=== HTTP Result ===");
    if (resultHTTP){
      Serial.println("HTTP Message Sent Successfully");
    } else {
      Serial.println("Error Sending HTTP Message");
    }
  }
  
}

String textTimes(String a, int b){
  String result = "";
  for (int i = 0; i < b; i++){
    result = result + a;
  }
  return result;
}

void setup() {
  Serial.begin(115200);
  connectWiFi();

  if (SEND_MQTT_DATA || RECIEVE_DATA_FROM_MQTT_SUB) connectAWS();
  if (INCLUDE_SENSOR_DATA) connectSensors();


  if (INCLUDE_DUMMY_INCREASING_DATA){
    doc['m'] = "a";
  }
}

void loop() {

  if (RECIEVE_DATA_FROM_MQTT_SUB) mqttClient.loop();

  if (messageCounter <= LIMIT) {
    Serial.printf("\n\n\nMessage Number %d sending\n", messageCounter);


    if (INCLUDE_DUMMY_INCREASING_DATA){
      if (messageCounter%(CHANGE_INCREASING_OVER_MESSAGES*2) == 0){
        String temp = doc['m'];
        doc['m'] = temp + textTimes("a",CHANGE_IN_BYTES);
      }
      
      if ((messageCounter+CHANGE_INCREASING_OVER_MESSAGES)%(CHANGE_INCREASING_OVER_MESSAGES*2)){
        m = m + textTimes("m",CHANGE_IN_BYTES);
        doc[m] = "b";
      }
    }
    publishMessage();

    // Print Payload info
    Serial.printf("PayloadSize(bytes): %u\n", measureJson(doc));
    Serial.printf("WifiStrength(dBm): %ld\n", getWifiStrength());

    Serial.printf("\nMessage Number %d sent\n\n\n", messageCounter);

    messageCounter++;

    if (messageCounter <= LIMIT) delay(LOOPTIME);
  }


  if (messageCounter == LIMIT + 1) {
    Serial.println();
    Serial.printf("Batch of %d is transferred. Iteration %d is completed, now going to iteration %d\n", LIMIT, iteration, iteration + 1);
    messageCounter++;  // Prevent re-entering this block
    endMsgTime = millis();
  }

  if (millis() > endMsgTime + TIMETORESENDMSGS && messageCounter > LIMIT && iteration <= ITERATIONS) {
    messageCounter = 1;
    doc.clear();
    iteration++;
  }

  if (iteration > ITERATIONS) {
    Serial.printf("Limit of %d iterations completed. Restart device to start scan again.\n", ITERATIONS);
  }
}

