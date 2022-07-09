#include "MServo.h"

MServo::MServo() {};


void MServo::write(uint8_t channel, uint16_t angle) {
    channel_last_called = channel;
    angle_last_called = angle;
}

void MServo::setSpeed(uint8_t channel, uint16_t speed) {
    channel_last_called = channel;
    speed_last_called = speed;
}

void MServo::setAccel(uint8_t channel, uint16_t accel) {
    channel_last_called = channel;
    accel_last_called = accel;
}
