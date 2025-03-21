#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;
char filename[20];

void setup() {
    Serial.begin(500000);
    while (!Serial) { ; }

    if (!SD.begin(chipSelect)) {
        while (1);  // Halt if SD card fails
    }

    int fileIndex = 1;
    do {
        snprintf(filename, sizeof(filename), "log_%d.txt", fileIndex++);
    } while (SD.exists(filename));
}

void loop() {
    if (Serial.available()) {
        File dataFile = SD.open(filename, FILE_WRITE);
        if (dataFile) {
            while (Serial.available()) {
                dataFile.write(Serial.read());  // Write incoming text directly to SD
            }
            dataFile.close();
        }
    }
}
