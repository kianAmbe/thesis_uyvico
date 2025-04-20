#include <SoftwareSerial.h>

SoftwareSerial pmsSerial(6, 7); // RX, TX

const int FRAME_SIZE = 32;
uint8_t buffer[FRAME_SIZE];

unsigned long lastAvgTime = 0;
const unsigned long AVG_INTERVAL = 60000; // 1 minute in milliseconds

// Accumulators
unsigned long pm10_total = 0;
unsigned long pm25_total = 0;
unsigned long pm100_total = 0;
int sampleCount = 0;

void setup() {
  Serial.begin(9600);
  pmsSerial.begin(9600);
  Serial.println("PMS Sensor Parser with 1-Minute Average");
}

void loop() {
  if (pmsSerial.available() >= FRAME_SIZE) {
    if (pmsSerial.read() == 0x42) {
      if (pmsSerial.read() == 0x4D) {
        buffer[0] = 0x42;
        buffer[1] = 0x4D;

        for (int i = 2; i < FRAME_SIZE; i++) {
          buffer[i] = pmsSerial.read();
        }

        // Extract PM values (standard)
        uint16_t pm10 = (buffer[10] << 8) | buffer[11];
        uint16_t pm25 = (buffer[12] << 8) | buffer[13];
        uint16_t pm100 = (buffer[14] << 8) | buffer[15];

        // Accumulate values
        pm10_total += pm10;
        pm25_total += pm25;
        pm100_total += pm100;
        sampleCount++;

        // Display current reading (optional)
        Serial.print("PM1.0: ");
        Serial.print(pm10);
        Serial.print(" µg/m³, PM2.5: ");
        Serial.print(pm25);
        Serial.print(" µg/m³, PM10: ");
        Serial.println(pm100);
      }
    }
  }

  // Check if it's time to compute and display the average
  if (millis() - lastAvgTime >= AVG_INTERVAL && sampleCount > 0) {
    Serial.println("\n--- 1-Minute Average ---");
    Serial.print("Avg PM1.0: ");
    Serial.print(pm10_total / sampleCount);
    Serial.print(" µg/m³, Avg PM2.5: ");
    Serial.print(pm25_total / sampleCount);
    Serial.print(" µg/m³, Avg PM10: ");
    Serial.print(pm100_total / sampleCount);
    Serial.println(" µg/m³\n");

    // Reset accumulators
    pm10_total = 0;
    pm25_total = 0;
    pm100_total = 0;
    sampleCount = 0;
    lastAvgTime = millis(); // reset timer
  }
}
