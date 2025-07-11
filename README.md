# ESP32S3 + MLX90614 Sleep Control

This project demonstrates how to interface the MLX90614 infrared temperature sensor with the ESP32-S3, and put both the ESP32 and MLX90614 into low-power sleep modes to save energy in wearable or remote sensing applications.

## Features
- Reads object temperature from MLX90614
- Puts MLX90614 into software sleep mode
- Disables I2C before deep sleep
- Powers off sensor via GPIO-controlled MOSFET
- Wakes up after 15 seconds

## Wiring
- **SDA**: GPIO 8
- **SCL**: GPIO 9
- **MOSFET Control Pin**: Define and connect to sensor power

## Library Dependencies
- [Adafruit_MLX90614](https://github.com/adafruit/Adafruit-MLX90614-Library)
- Wire (built-in)

## Power Saving Strategy
1. Sends 0xFF sleep command to MLX90614
2. Powers off sensor via MOSFET
3. Disables I2C
4. Puts ESP32 in deep sleep

## License
MIT
