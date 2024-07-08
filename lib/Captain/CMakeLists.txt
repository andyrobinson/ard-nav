cmake_minimum_required(VERSION 3.10)
project(CaptainTest VERSION 1.1)
set(TestName "CaptainTest")
set(Includes "../Waypoint" "../Position" "../Stubs/Navigator")
set(Sources "CaptainTest.cpp" "Captain.cpp" "../Stubs/Navigator/Navigator.cpp")
include("../common.cmake")


