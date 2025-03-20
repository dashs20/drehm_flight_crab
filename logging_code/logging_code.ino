#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;         // SD card chip select pin
const char *filename = "datalog.txt";
const unsigned int bufferSize = 128;
char dataBuffer[bufferSize];
unsigned int bufferIndex = 0;

void setup() {
  Serial.begin(115200);
  // Wait for the serial port to connect (for boards with native USB, if needed)
  while (!Serial) { ; }

  Serial.println("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    while (1); // halt execution if SD card fails
  }
  Serial.println("SD card initialized.");
}

void loop() {
  // Read available data from the Serial port
  while (Serial.available() > 0) {
    char c = Serial.read();
    dataBuffer[bufferIndex++] = c;

    // When a newline or near-buffer-full is reached, write the data to the SD card.
    if (c == '\n' || bufferIndex >= bufferSize - 1) {
      dataBuffer[bufferIndex] = '\0'; // Null-terminate the string

      // Open the file in append mode
      File dataFile = SD.open(filename, FILE_WRITE);
      if (dataFile) {
        dataFile.print(dataBuffer);
        dataFile.close();  // Make sure to close the file to save data
        Serial.print("Logged: ");
        Serial.print(dataBuffer); // Optional: print logged data for debugging
      } else {
        Serial.println("Error opening datalog.txt for writing");
      }
      bufferIndex = 0; // Reset the buffer index after writing
    }
  }
}
