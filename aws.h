



// AWS Constants

#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"
#define SERVER_PORT             4080




void messageHandler(char* topic, byte* payload, unsigned int length) {
  Serial.print("MQTT Message on topic: ");
  Serial.println(topic);

  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, payload, length);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  const char* msg = doc["message"];
  Serial.print("Message content: ");
  Serial.println(msg);
}



void connectAWS() {


  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  mqttClient.setServer(AWS_IOT_ENDPOINT, 8883);
  mqttClient.setCallback(messageHandler);

  Serial.print("Connecting to AWS IoT");
  int retries = 0;
  while (!mqttClient.connect(THINGNAME) && retries < 10) {
    Serial.print(".");
    delay(1000);
    retries++;
  }

  if (mqttClient.connected()) {
    Serial.println("\nConnected to AWS IoT");
    mqttClient.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  } else {
    Serial.println("\nAWS IoT Connection Failed!");
    Serial.print("MQTT state: ");
    Serial.println(mqttClient.state());
  }
}


bool sendMQTT(StaticJsonDocument<4096> doc){

  char buffer[4096];
  size_t payloadSize = serializeJson(doc, buffer);

  if (!mqttClient.connected()) {
    Serial.println("MQTT not connected at publish time!");
    Serial.print("MQTT state: ");
    Serial.println(mqttClient.state());
  }

  bool result = mqttClient.publish(AWS_IOT_PUBLISH_TOPIC, buffer);

  Serial.println("\n=== PUBLISHED PAYLOAD ===");
  Serial.println(buffer);
  Serial.printf("Payload size: %u bytes\n", payloadSize);
  Serial.println("==========================\n");

  return result; 
}