#include "secret.h"

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <time.h>  // for configTime() and time()
#include <PubSubClient.h>


WiFiClientSecure net;
PubSubClient mqttClient(net);

uint64_t  startEpochOffset = 0;


// Public download test file (1MB)
const char* downloadUrl = "http://speedtest.tele2.net/1MB.zip";

// Replace this with your own test POST server URL
//const char* uploadUrl = "https://c238fcn8lf.execute-api.ap-southeast-2.amazonaws.com/WriteHTTPDynamoData";  // Change this!

void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Use native NTP (better than NTPClient)
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");

  // Wait for time to sync
  while (time(nullptr) < 24 * 3600) {
    delay(500);
    Serial.print(".");
  }

  // Once synced, calculate offset
  startEpochOffset = static_cast<int64_t>(time(nullptr)) * 1000LL - (int64_t)millis();
  Serial.printf("\nEpoch offset (ms): %lld\n", startEpochOffset);
}

long getWifiStrength(){
  return WiFi.RSSI();
}


float getDownloadSpeedKbps() {
  HTTPClient http;
  http.begin(downloadUrl);

  unsigned long startTime = millis();
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    WiFiClient* stream = http.getStreamPtr();
    uint8_t buffer[128];
    size_t totalBytes = 0;

    while (http.connected() && stream->available()) {
      int len = stream->readBytes(buffer, sizeof(buffer));
      totalBytes += len;
    }

    unsigned long endTime = millis();
    http.end();

    float timeSec = (endTime - startTime) / 1000.0;
    return (totalBytes * 8) / (timeSec * 1000.0);  // kbps
  }

  http.end();
  return -1;
}

// 📤 Measure Upload Speed
float getUploadSpeedKbps() {
  const char* uploadUrl = "	https://webhook.site/e19da935-e9b7-4075-a00e-2cdafe8c361b"; // 🔁 Replace this!

  HTTPClient http;
  http.begin(uploadUrl);
  http.addHeader("Content-Type", "application/octet-stream");

  const int dataSize = 8 * 1024;  // ✅ Safer buffer size: 8 KB
  uint8_t* buffer = (uint8_t*)malloc(dataSize);
  if (!buffer) {
    Serial.println("Failed to allocate upload buffer.");
    return -1;
  }

  memset(buffer, 'A', dataSize);  // Fill with dummy data

  Serial.println("Starting upload speed test...");

  unsigned long startTime = millis();
  int httpCode = http.POST(buffer, dataSize);
  unsigned long endTime = millis();

  free(buffer);
  http.end();

  if (httpCode > 0) {
    float timeSec = (endTime - startTime) / 1000.0;
    float speedKbps = (dataSize * 8.0) / (timeSec * 1000.0); // kbps
    Serial.printf("Upload took %.2f seconds, speed = %.2f kbps\n", timeSec, speedKbps);
    return speedKbps;
  } else {
    Serial.printf("Upload failed! HTTP code: %d\n", httpCode);
    return -1;
  }
}
