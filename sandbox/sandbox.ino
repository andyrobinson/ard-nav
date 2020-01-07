// Adafruit SSD1306 - Version: Latest
#include <SPI.h>
#include <Wire.h>
//#include <WindSensor.h>
#include <Compass.h>
#include <math.h>
//#include <Servo.h>
#include <Angle.h>

// Serial2 pin and pad definitions (in Arduino files Variant.h & Variant.cpp)
#define PIN_SERIAL2_RX       (34ul)               // Pin description number for PIO_SERCOM on D12
#define PIN_SERIAL2_TX       (36ul)               // Pin description number for PIO_SERCOM on D10
#define PAD_SERIAL2_TX       (UART_TX_PAD_2)      // SERCOM pad 2
#define PAD_SERIAL2_RX       (SERCOM_RX_PAD_3)    // SERCOM pad 3

// Instantiate the Serial2 class
Uart Serial2(&sercom1, PIN_SERIAL2_RX, PIN_SERIAL2_TX, PAD_SERIAL2_RX, PAD_SERIAL2_TX);

void SERCOM1_Handler()    // Interrupt handler for SERCOM1
{
  Serial2.IrqHandler();
}

#define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F"
#define PMTK_SET_NMEA_UPDATE_5HZ  "$PMTK220,200*2C"
#define PMTK_SET_NMEA_UPDATE_10HZ "$PMTK220,100*2F"

// turn on only the second sentence (GPRMC)
#define PMTK_SET_NMEA_OUTPUT_RMCONLY "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"
// turn on GPRMC and GGA
#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
// turn on ALL THE DATA
#define PMTK_SET_NMEA_OUTPUT_ALLDATA "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
// turn off output
#define PMTK_SET_NMEA_OUTPUT_OFF "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"

#define PMTK_Q_RELEASE "$PMTK605*31"


using namespace Angle;
//WindSensor windsensor;
Compass compass;
//Servo servo;
//int servoposition = 0;
//int servoinc = 1;


void setup() {
  while (!Serial); // wait for Serial to be ready

  Serial.begin(9600);
  // Serial2.begin(9600);
  // delay(2000);

  // Serial.println("Get version!");
  // Serial2.println(PMTK_Q_RELEASE);

  // you can send various commands to get it started
  //mySerial.println(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // Serial2.println(PMTK_SET_NMEA_OUTPUT_ALLDATA);

  // Serial2.println(PMTK_SET_NMEA_UPDATE_1HZ);
  //  windsensor.begin();
   compass.begin();
  //  servo.attach(5);
}

void loop() {

  // if (Serial2.available()) {
  //   //byte byteRead = Serial2.read();
  //   char c = Serial2.read();
  //   Serial.write(c);
  // }

  char buf[20];
  //  angle wind = windsensor.relative();
  MagResult bearing = compass.bearing();
  MagResult accel = compass.accel();
  int heading = (360 + round(57.2958 * atan2((double) bearing.y, (double) (bearing.x-100)))) % 360;

  double roll = atan2((double)accel.y, (double)accel.z);
  double pitch = atan2((double) - accel.x, (double) accel.z); // reversing x accel makes it work
  double sin_roll = sin(roll);
  double cos_roll = cos(roll);
  double cos_pitch = cos(pitch);
  double sin_pitch = sin(pitch);

  double x_final = ((double) (bearing.x-100)) * cos_pitch + ((double) bearing.y) * sin_roll * sin_pitch + ((double) bearing.z) * cos_roll * sin_pitch;
  double y_final = ((double) bearing.y) * cos_roll - ((double) bearing.z) * sin_roll;
  int tiltadjust = (360 + round(57.2958 * (atan2(y_final, x_final)))) % 360;

  //
  //  sprintf(buf, "Wind: %d",wind);
  //  Serial.println(buf);
  sprintf(buf, "Comp: %d Tilt: %d", heading, tiltadjust);
  Serial.println(buf);
  sprintf(buf, "Accel: %d %d %d", accel.x, accel.y, accel.z);
  Serial.println(buf);
  sprintf(buf, "Bearing: %d %d %d", bearing.x, bearing.y, bearing.z);
  Serial.println(buf);
//  sprintf(buf, "%d %d %d", bearing.x, bearing.y, bearing.z);
//  Serial.println(buf);
  //
  //  servoposition = servoposition + servoinc;
  //
  //  if (servoposition % 5==0) {
  //    servo.write(servoposition);
  //  }
  //
  //  if (servoposition <= 0 || servoposition >= 180) {
  //    servoinc = -servoinc;
  //  }
  //
  delay(1000);

}
