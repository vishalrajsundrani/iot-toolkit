// Just change below constanst captalized variables and secret.h to use this testing script


int MQTT_DELAY_OVER_ERROR_IN_MESSAGE = 1000; // Millisecond
int DOCSIZE = 1024;  // Maximum data size to send adn reserve in ESP32

// Most of ESP32 Errors came because of these two upper variables. So, tweak them (decrease DOCSIZE or increase DELAY time) to fix the issue if came.

int LIMIT = 10;  // Number of messages per iteration
unsigned long TIMETORESENDMSGS = 60 * 1000;  // Millisecond Wait between iterations
int ITERATIONS = 5;  // Total number of iterations
int LOOPTIME = 1000;  // Millisecond delay between messages

// What Data to sent
bool INLCUDE_MANDATORY_DATA = true;
bool INCLUDE_SENSOR_DATA = false;
bool INCLUDE_DUMMY_STATIONARY_DATA = true;

bool INCLUDE_DUMMY_INCREASING_DATA = false;
int CHANGE_INCREASING_OVER_MESSAGES = 10;
int CHANGE_IN_BYTES = 10;

// MQTT Setup
bool SEND_MQTT_DATA = true;
bool RECIEVE_DATA_FROM_MQTT_SUB = false;

// HTTP Setup
bool SEND_HTTP_DATA = true;
String API_URL = "http://13.211.139.52:3000/data";
String YOUR_HTTP_RESPONSE = "{}";






// Do not change code dow below

#include <math.h>

if (SEND_HTTP_DATA){
    #include <HTTPClient.h>
}


#include <ArduinoJson.h>

if (INCLUDE_SENSOR_DATA){
    #include <DHT.h>
    #include <Wire.h>
    #include <Adafruit_MPU6050.h>
    #include <Adafruit_Sensor.h>
}


#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <time.h>  // for configTime() and time()
#include <PubSubClient.h>