#include <SPI.h>
#include <SD.h>
#include <Compass.h>
#include <Timer.h>
#include <I2C.h>

#define CHIP_SELECT 4

#define CALLIB_X_CORRECTION -70
#define CALLIB_Y_CORRECTION -80
#define CALLIB_Z_CORRECTION 300

int pos = 0;    // variable to store the servo position
File dataFile;

I2C i2c;
Timer timer;
Compass compass(&i2c, &timer);

MagResult bearing, accel;
double roll,pitch, sin_roll, cos_roll, cos_pitch, sin_pitch, x_final, y_final;
uangle final_bearing;
short tiltadjust;

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
    final_bearing = compass.bearing();

    if(!bearing.valid || !accel.valid) {
        x_final = -1.0;
        y_final = -1.0;
        tiltadjust = -1;
    } else {
        roll = atan2((double)accel.y, (double)accel.z);
        pitch = atan2((double) accel.x, (double) accel.z);
        sin_roll = sin(roll);
        cos_roll = cos(roll);
        cos_pitch = cos(pitch);
        sin_pitch = sin(pitch);

        double x = (double) (bearing.x + CALLIB_X_CORRECTION);
        double y = (double) (-bearing.y + CALLIB_Y_CORRECTION);
        double z = (double) (-bearing.z + CALLIB_Z_CORRECTION);

        x_final = x * cos_pitch + y*sin_roll*sin_pitch + z*cos_roll*sin_pitch;
        y_final = y * cos_roll - z*sin_roll;
        tiltadjust = (360 + (short) round(57.2958 * (atan2(y_final,x_final)))) % 360;

    }

    dataFile = SD.open("callib3.csv", FILE_WRITE);

    if (dataFile) {
      dataFile.print(bearing.x);dataFile.print(",");
      dataFile.print(bearing.y);dataFile.print(",");
      dataFile.print(bearing.z);dataFile.print(",");
      dataFile.print(accel.x);dataFile.print(",");
      dataFile.print(accel.y);dataFile.print(",");
      dataFile.print(accel.z);dataFile.print(",");
      dataFile.print(final_bearing);

      dataFile.print(",,");
      dataFile.print(x_final);dataFile.print(",");
      dataFile.print(y_final);dataFile.print(",");
      dataFile.println(tiltadjust);
      dataFile.close();
    }

}
