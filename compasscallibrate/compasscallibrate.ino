#include <SPI.h>
#include <SD.h>
#include <Compass.h>
#include <Timer.h>
#include <I2C.h>

#define CHIP_SELECT 4

int pos = 0;    // variable to store the servo position
File dataFile;
char dataString[20] = "callib.csv";

I2C i2c;
Timer timer;
Compass compass(&i2c, &timer);

MagResult bearing, accel;
double roll,pitch, sin_roll, cos_roll, cos_pitch, sin_pitch, x_final, y_final;

void setup() {
    i2c.begin();
    compass.begin();

    if (!SD.begin(CHIP_SELECT)) {
      Serial.println("Card failed, or not present");
        while(true) {};
    }

}

void loop() {
    delay(100);

    bearing = compass.raw_bearing();
    accel = compass.raw_accel();

    if(!bearing.valid || !accel.valid) {
        x_final = -1.0;
        y_final = -1.0;
    } else {
        roll = atan2((double)accel.y, (double)accel.z);
        pitch = atan2((double) -accel.x, (double) accel.z); // reversing x accel makes it work
        sin_roll = sin(roll);
        cos_roll = cos(roll);
        cos_pitch = cos(pitch);
        sin_pitch = sin(pitch);

        x_final = ((double) bearing.x) * cos_pitch + ((double) bearing.y)*sin_roll*sin_pitch+((double) bearing.z)*cos_roll*sin_pitch;
        y_final = ((double) bearing.y) * cos_roll-((double) bearing.z) * sin_roll;
    }

    dataFile = SD.open("callib.csv", FILE_WRITE);

    if (dataFile) {
        dataFile.print(x_final);
        dataFile.print(",");
        dataFile.println(y_final);
        dataFile.close();
    }

}
