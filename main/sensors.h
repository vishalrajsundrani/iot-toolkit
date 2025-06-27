



// ──────────  Sensor Pins  ──────────
#define IR_SENSOR_PIN       32
#define VIBRATION_PIN       33
#define DHT_PIN             27
#define DHT_TYPE            DHT11
#define LIGHT_SENSOR_PIN    34
#define TILT_SENSOR_PIN     15

// I2C Pins (default on ESP32) for IMU (MPU-6050)
#define SDA_PIN 21
#define SCL_PIN 22

DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_MPU6050 mpu;



// ────────── Global variables to store accelerometer and gyroscope values ──────────
float accX = 0.0, accY = 0.0, accZ = 0.0;      // Accelerometer data (m/s²)
float gyroX = 0.0, gyroY = 0.0, gyroZ = 0.0;    // Gyroscope data (deg/s)
float angleX = 0.0, angleY = 0.0, angleZ = 0.0; // Angle values (degrees)
float tempm = 0.0;


void connectSensors(){
  // Start I2C communication with the specified SDA and SCL pins
  Wire.begin(SDA_PIN, SCL_PIN);

  // Initialize the MPU6050 sensor
  Serial.println("Initializing MPU6050...");
  while (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 sensor");
    delay(500);
  }

  Serial.println("MPU6050 sensor initialized successfully!");

  // Optionally, you can set the sensor range for accelerometer and gyroscope
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);  // 2g, 4g, 8g, 16g
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);  // 250, 500, 1000, 2000 degrees per second

  // You can set the filters here as well if needed (optional)
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);  // Adjust bandwidth for noise filtering
  
  Serial.println("MPU6050 sensor ready to read data!");



  // Initialize sensor pins
  pinMode(VIBRATION_PIN, INPUT);
  pinMode(TILT_SENSOR_PIN,    INPUT);
  dht.begin();

}



StaticJsonDocument<4096> get_sensorsData(){
  // Read sensors
  int   irRaw    = analogRead(IR_SENSOR_PIN);
  float distance = irRaw / 9.766;     // in cm
  int   vibVal   = digitalRead(VIBRATION_PIN);
  int   tiltVal  = digitalRead(TILT_SENSOR_PIN);
  int   lightVal = analogRead(LIGHT_SENSOR_PIN);
  float temp     = dht.readTemperature();
  float hum      = dht.readHumidity();


  // Declare variables to hold accelerometer and gyroscope data
  sensors_event_t a, g, tempmo;
  
  // Get accelerometer and gyroscope data
  mpu.getEvent(&a, &g, &tempmo);

  // Store accelerometer data (m/s²)
  accX = a.acceleration.x;
  accY = a.acceleration.y;
  accZ = a.acceleration.z;

  // Store gyroscope data (deg/s)
  gyroX = g.gyro.x * 180.0 / PI;
  gyroY = g.gyro.y * 180.0 / PI;
  gyroZ = g.gyro.z * 180.0 / PI;

  // Calculate angle values (in degrees) using accelerometer data
  angleX = atan2(accY, sqrt(accX * accX + accZ * accZ)) * 180.0 / PI;
  angleY = atan2(-accX, sqrt(accY * accY + accZ * accZ)) * 180.0 / PI;
  angleZ = atan2(accZ, sqrt(accX * accX + accY * accY)) * 180.0 / PI;


  StaticJsonDocument<4096> doc;
  
  doc["device"]         = "ESP32_Client";
  doc["deviceID"]       = 123;
  doc["status"]         = "active";
  doc["distance_cm"]    = distance;
  doc["vibration"]      = (vibVal == HIGH) ? "Detected" : "None";
  doc["tilt"]           = (tiltVal == LOW)  ? "not tilted" : "tilted";
  doc["light_intensity"]= lightVal;
  doc["accX"]           = accX;
  doc["accY"]           = accY;
  doc["accZ"]           = accZ;
  doc["gyroX"]           = gyroX;
  doc["gyroY"]           = gyroY;
  doc["gyroZ"]           = gyroZ;
  doc["angleX"]           = angleX;
  doc["angleY"]           = angleY;
  doc["angleZ"]           = angleZ;
  


  if (isnan(temp) || isnan(hum)) {
    doc["temperature"] = "error";
    doc["humidity"]    = "error";
  } else {
    doc["temperature"] = temp;
    doc["humidity"]    = hum;
  }

  return doc;

}
