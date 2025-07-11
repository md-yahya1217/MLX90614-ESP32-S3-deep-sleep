#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <esp_sleep.h>

// GPIOs
// #define SENSOR_SWITCH_PIN 6     // Controls MOSFET (LOW = ON)
#define SDA_PIN 8
#define SCL_PIN 9

// Sleep time
#define TIME_TO_SLEEP 15        // in seconds
#define uS_TO_S_FACTOR 1000000ULL

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void goToSleep() {
  // Step 4: Put MLX90614 into software sleep (send command 0xFF)
  Serial.println("Sending sleep command to MLX90614...");
  Wire.beginTransmission(0x5A);
  Wire.write(0xFF);  // Sleep command
  Wire.endTransmission();
  delay(100);

  // Step 5: Disable I2C and set GPIOs low
  Wire.end();
  pinMode(SDA_PIN, OUTPUT);
  pinMode(SCL_PIN, OUTPUT);
  digitalWrite(SDA_PIN, LOW);
  digitalWrite(SCL_PIN, LOW);

  // Step 6: Power off sensor
  digitalWrite(SENSOR_SWITCH_PIN, HIGH);  // HIGH = MOSFET OFF
  Serial.println(" Sensor OFF. ESP32 sleeping...");
  delay(100);

  // Step 7: Deep sleep
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}

void wakeMLX90614(uint8_t sclPin, uint8_t sdaPin) {
  Wire.end();  // Release I2C
  pinMode(sclPin, OUTPUT);
  digitalWrite(sclPin, HIGH);
  pinMode(sdaPin, OUTPUT);
  digitalWrite(sdaPin, LOW);
  delay(100);  // ≥80ms
  digitalWrite(sdaPin, HIGH);
  digitalWrite(sclPin, HIGH);
}

void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(SENSOR_SWITCH_PIN, OUTPUT);
  digitalWrite(SENSOR_SWITCH_PIN, LOW);  // Power ON sensor
  delay(50);

  wakeMLX90614(SCL_PIN, SDA_PIN);
  delay(10);

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!mlx.begin()) {
    Serial.println("MLX90614 not found!");
    goToSleep();
    return;
  }

  Serial.println("MLX90614 found. Reading...");
  float temp = mlx.readObjectTempC();
  Serial.print("Object Temp: ");
  Serial.print(temp);
  Serial.println(" *C");

  delay(10000);
  goToSleep();
}

void loop() {
  // never used
}
