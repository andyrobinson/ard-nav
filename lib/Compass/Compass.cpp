#include "Compass.h"

Compass::Compass(){}
Compass::Compass(I2C* i2cp, Timer* timerp):i2c(i2cp),timer(timerp),reset_pause(COMPASS_INITIAL_RESET_PAUSE_MS),reset_count(0),reset_start(0) {}

void Compass::begin() {
 #ifdef ARDUINO
  pinMode(COMPASS_POWER_PIN, OUTPUT);
  digitalWrite(COMPASS_POWER_PIN, HIGH);
 #endif

  timer->wait(COMPASS_INITIAL_PAUSE);

  // Enable the compass
  i2c->write_register_value(COMPASS_COMPASS_I2C_ADDRESS, COMPASS_REGISTER_ENABLE, 0x00);
  // Enable the accelerometer
  i2c->write_register_value(COMPASS_ACCEL_I2C_ADDRESS, COMPASS_ACCEL_CTRL_REG1_A, 0x27);

  last_read_time = timer->milliseconds() - (COMPASS_CACHE_TTL_MS + 1);

  if (reset_start == 0) reset_start = timer->milliseconds();
}

uangle Compass::bearing() {

   if ((timer->milliseconds() - last_read_time) < COMPASS_CACHE_TTL_MS){
     Serial.print("Cached ");Serial.println(tiltadjust);
     return tiltadjust;
   }

   if (i2c->err_percent() >= COMPASS_RESET_ERROR_THRESHOLD) reset();

   if (i2c->err_percent() == 0) reset_pause = COMPASS_INITIAL_RESET_PAUSE_MS;

   MagResult bearing = raw_bearing();
   MagResult accel = raw_accel();

   if(!bearing.valid || !accel.valid) return ANGLE_ERROR;

   double roll = atan2((double)accel.y, (double)accel.z);
   double pitch = atan2((double) -accel.x, (double) accel.z); // reversing x accel makes it work
   double sin_roll = sin(roll);
   double cos_roll = cos(roll);
   double cos_pitch = cos(pitch);
   double sin_pitch = sin(pitch);

   double x_final = ((double) bearing.x) * cos_pitch + ((double) bearing.y)*sin_roll*sin_pitch+((double) bearing.z)*cos_roll*sin_pitch;
   double y_final = ((double) bearing.y) * cos_roll-((double) bearing.z) * sin_roll;
   tiltadjust = (360 + (short) round(57.2958 * (atan2(y_final,x_final)))) % 360;

   last_read_time = timer->milliseconds();

   Serial.print("Fresh ");Serial.println(tiltadjust);

   return tiltadjust;
}

MagResult Compass::raw_bearing() {
  uint8_t endTransResult = i2c->write_register((uint8_t) COMPASS_COMPASS_I2C_ADDRESS, COMPASS_REGISTER_X_HIGH);

  if (endTransResult) return {0,0,0,false};

  i2c->requestFrom((uint8_t) COMPASS_COMPASS_I2C_ADDRESS, (uint8_t) 6);

  if (!i2c->wait_for_data(6)) return {0,0,0,false};

  uint8_t xhi = i2c->read();
  uint8_t xlo = i2c->read();
  uint8_t zhi = i2c->read();
  uint8_t zlo = i2c->read();
  uint8_t yhi = i2c->read();
  uint8_t ylo = i2c->read();

  return {hilow_toint(xhi,xlo) + COMPASS_X_CORRECTION, hilow_toint(yhi,ylo), hilow_toint(zhi,zlo), true};
}

MagResult Compass::raw_accel() {
  uint8_t endTransResult = i2c->write_register((uint8_t) COMPASS_ACCEL_I2C_ADDRESS, ACCEL_REGISTER_OUT_X_L_A | 0x80);

  if (endTransResult) return {0,0,0,false};

  i2c->requestFrom((uint8_t) COMPASS_ACCEL_I2C_ADDRESS, (uint8_t) 6);

  if (!i2c->wait_for_data(6)) return {0,0,0,false};

  uint8_t xlo = i2c->read();
  uint8_t xhi = i2c->read();
  uint8_t ylo = i2c->read();
  uint8_t yhi = i2c->read();
  uint8_t zlo = i2c->read();
  uint8_t zhi = i2c->read();

  return {hilow_toint(xhi,xlo), hilow_toint(yhi,ylo), hilow_toint(zhi,zlo),true};
}

int Compass::hilow_toint(uint8_t high, uint8_t low) {
  return (int16_t)((uint16_t) low | ((uint16_t) high << 8));
}

int Compass::err_percent() {
   return i2c->err_percent();
}

long Compass::resets_per_hour() {
  long seconds = constrain((timer->milliseconds() - reset_start)/1000,1l,timer->milliseconds());
  long resets_per_hour = (COMPASS_SECONDS_PER_HOUR * reset_count)/seconds;
  return constrain(resets_per_hour, 0l, COMPASS_MAX_RESETS);
}

void Compass::reset() {
  #ifdef ARDUINO
  digitalWrite(COMPASS_POWER_PIN, LOW);
  #endif
  timer->wait(reset_pause);
  // Seems to recover better WITHOUT a Wire reset

  reset_count++;
  reset_count = constrain(reset_count,0l,COMPASS_MAX_RESETS * COMPASS_SECONDS_PER_HOUR);
  begin();

  // once we have reached the limit, don't increase the pause further or reset errors
  if (reset_pause <= COMPASS_MAX_RESET_PAUSE_MS) {
    i2c->set_error_percent(1); // one percent, so that we don't reset the back-off
    reset_pause = reset_pause * 2; // exponential back-off
  }
}
