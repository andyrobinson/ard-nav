#include "Battery.h"

Battery::Battery() {}

Battery::Battery(int (*analogPinFn)(uint8_t), Timer *timerp):
  buffer_index(0),readAnalogPin(analogPinFn),timer(timerp),last_read(0){
  for (int i=0;i<SAMPLES;i++) readings_buffer[i]=0;
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
  int min = MAX_ANALOG;
  for (int i=0; i < SAMPLES; i++) {
    int current = readings_buffer[i];
    min = (current < min && current > 0) ? current : min;
  }
  return min;
}

uint16_t Battery::raw_max(uint8_t batt) {
  int max = 0;
  for (int i=0; i < SAMPLES; i++) {
    max = (readings_buffer[i] > max) ? readings_buffer[i] : max;
  }
  return max;
}

void Battery::add_reading() {
  if (timer->milliseconds() > last_read + READ_DELAY) {
    uint16_t raw_reading = (uint16_t) readAnalogPin((uint8_t) LIPO1);
    readings_buffer[buffer_index] = raw_reading;
    buffer_index = (buffer_index + 1) % SAMPLES; // circular buffer
    last_read = timer->milliseconds();
  }
}

float Battery::to_volts(uint16_t reading) {
  float result = 6.6 * ((float) reading)/MAX_ANALOG; // we have divided the voltage to get it below 3.3v
  return result;

}
