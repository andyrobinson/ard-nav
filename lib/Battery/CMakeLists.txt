cmake_minimum_required(VERSION 3.10)
project(BatteryTest VERSION 1.1)
set(TestName "BatteryTest")
set(Includes "../Stubs/Timer" "../Stubs/Arduino" "stdint.h")
set(Sources "BatteryTest.cpp" "Battery.cpp" "../Stubs/Timer/Timer.cpp" "../Stubs/Arduino/Arduino.cpp")
include("../common.cmake")


