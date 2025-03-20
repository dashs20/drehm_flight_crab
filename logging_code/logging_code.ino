#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;    // Updated: CS is now on pin 4
File logFile;

// Buffer settings: adjust based on available RAM and expected message size
#define BUFFER_SIZE 512
char buffer[BUFFER_SIZE];
int bufferIndex = 0;

// Flush interval (in milliseconds) to force a write even if the buffer isn’t full
const unsigned long flushInterval = 500;
unsigned long lastFlushTime = 0;

void setup() {
  // Initialize the hardware Serial at 500000 baud.
  // This port is used for both receiving data and (if connected via USB) for debugging.
  Serial.begin(500000);
  
  // Wait for the serial port to connect (optional, for some boards)
  while (!Serial) { }

  Serial.println("Initializing SD card...");

  delay(5000);

  // Initialize the SD card using chipSelect on pin 4
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    while (1);  // Halt execution if the SD card isn't working
  }
  Serial.println("SD card initialized.");

  // Open (or create) the log file for appending
  logFile = SD.open("datalog.txt", FILE_WRITE);
  if (!logFile) {
    Serial.println("Error opening datalog.txt");
    while (1);
  }
  
  lastFlushTime = millis();
}

void loop() {
  // Read incoming serial data as long as data is available
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (bufferIndex < BUFFER_SIZE - 1) {  // Reserve space for null terminator
      buffer[bufferIndex++] = c;
    } else {
      // Buffer full: flush immediately to free up space
      buffer[bufferIndex] = '\0'; // Null-terminate the string
      logFile.print(buffer);
      logFile.flush();  // Write immediately to the SD card
      bufferIndex = 0;
    }
  }
  
  // Flush the buffer periodically even if it isn’t full
  if (millis() - lastFlushTime >= flushInterval) {
    if (bufferIndex > 0) {
      buffer[bufferIndex] = '\0';
      logFile.print(buffer);
      logFile.flush();
      bufferIndex = 0;
    }
    lastFlushTime = millis();
  }
}
