#include "Battery.h"

Battery::Battery() {}

Battery::Battery(int (*analogPinFn)(uint8_t), Timer *timerp):
  buffer_index(0),readAnalogPin(analogPinFn),timer(timerp),last_read(0){
  for (int i=0;i<SAMPLES;i++) {
    readings_buffer[0][i]=0;
    readings_buffer[1][i]=0;
  }
}

float Battery::lipomaxv(uint8_t batt) {
  add_reading();
  return to_volts(raw_max(batt));
}

float Battery::lipominv(uint8_t batt) {
  add_reading();
  return to_volts(raw_min(batt));
}

uint16_t Battery::raw_min(uint8_t batt) {
  uint16_t min = MAX_ANALOG;
  for (int i=0; i < SAMPLES; i++) {
    int current = readings_buffer[constrain(batt,(uint8_t) 0,(uint8_t) 1)][i];
    min = (current < min && current > 0) ? current : min;
  }
  return min;
}

uint16_t Battery::raw_max(uint8_t batt) {
  uint16_t max = 0;
  for (int i=0; i < SAMPLES; i++) {
    int current = readings_buffer[constrain(batt,(uint8_t)0,(uint8_t)1)][i];
    max = (current > max) ? current : max;
  }
  return max;
}

void Battery::add_reading() {
  if (timer->milliseconds() > last_read + READ_DELAY) {
    uint16_t raw_reading1 = (uint16_t) readAnalogPin((uint8_t) LIPO1);
    uint16_t raw_reading2 = (uint16_t) readAnalogPin((uint8_t) LIPO2);

    readings_buffer[0][buffer_index] = raw_reading1;
    readings_buffer[1][buffer_index] = raw_reading2;
    
    buffer_index = (buffer_index + 1) % SAMPLES; // circular buffer
    last_read = timer->milliseconds();
  }
}

float Battery::to_volts(uint16_t reading) {
  float result = 6.6 * ((float) reading)/MAX_ANALOG; // we have divided the voltage to get it below 3.3v
  return result;

}
