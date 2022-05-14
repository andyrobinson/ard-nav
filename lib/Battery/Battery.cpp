#include "Battery.h"

Battery::Battery() {}

Battery::Battery(int (*analogPinFn)(uint8_t)):buffer_index(0),readAnalogPin(analogPinFn){
  for (int i=0;i<SAMPLES;i++) readings_buffer[i]=0;
}

float Battery::lipo1maxv() {
  add_reading(readAnalogPin(LIPO1));
  return to_volts(max_reading());
}

int Battery::max_reading() {
  int max = 0;
  for (int i=0; i < SAMPLES; i++) {
    max = (readings_buffer[i] > max) ? readings_buffer[i] : max;
  }
  return max;
}

void Battery::add_reading(int reading) {
  readings_buffer[buffer_index] = reading;
  buffer_index = (buffer_index + 1) % SAMPLES; // circular buffer
}

float Battery::to_volts(int reading) {
  float result = 6.6 * ((float) reading)/MAX_ANALOG; // we have divided the voltage to get it below 3.3v
  return result;

}
