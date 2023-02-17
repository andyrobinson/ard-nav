#include "Battery.h"

Battery::Battery() {}

Battery::Battery(int (*analogPinFn)(uint8_t), Timer *timerp):
  buffer_index(0),readAnalogPin(analogPinFn),timer(timerp),last_read(0){
  for (int i=0;i<SAMPLES;i++) readings_buffer[i]=0;
}

float Battery::lipo1maxv() {
  add_reading();
  return to_volts(max_reading());
}

float Battery::lipo1minv() {
  add_reading();
  return to_volts(min_reading());
}

int Battery::min_reading() {
  int min = MAX_ANALOG;
  for (int i=0; i < SAMPLES; i++) {
    int current = readings_buffer[i];
    min = (current < min && current > 0) ? current : min;
  }
  return min;
}

int Battery::max_reading() {
  int max = 0;
  for (int i=0; i < SAMPLES; i++) {
    max = (readings_buffer[i] > max) ? readings_buffer[i] : max;
  }
  return max;
}

void Battery::add_reading() {
  if (timer->milliseconds() > last_read + READ_DELAY) {
    int raw_reading = readAnalogPin((uint8_t) LIPO1);
    readings_buffer[buffer_index] = raw_reading;
    buffer_index = (buffer_index + 1) % SAMPLES; // circular buffer
    last_read = timer->milliseconds();
  }
}

float Battery::to_volts(int reading) {
  float result = 6.6 * ((float) reading)/MAX_ANALOG; // we have divided the voltage to get it below 3.3v
  return result;

}
