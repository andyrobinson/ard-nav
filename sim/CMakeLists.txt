cmake_minimum_required(VERSION 3.10)
project(Sim VERSION 1.1)

set(Includes Logger)

set(LibClasses Angle Globe Helm Tacker Navigator Captain Utility RotaryPID Sail Rudder)
set(LibIncludes Position Utility Routes Windrange Waypoint IHelm ${LibClasses})
list(TRANSFORM LibIncludes PREPEND "../lib/")
list(TRANSFORM LibClasses REPLACE "([^;]+)" "../lib/\\1/\\1.cpp")

set(Classes Boat Timer ConsoleLogger Gps Compass Switches WindSensor MServo Arduino LiftDrag SatComm)
set(ClassSources ${Classes})
list(TRANSFORM ClassSources REPLACE "([^;]+)" "\\1/\\1.cpp")

set(Sources "sim.cpp" ${ClassSources})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CSS_STANDARD_REQUIRED True)

add_executable(Sim ${Sources} ${LibClasses})
target_include_directories(Sim PUBLIC ${LibIncludes} ${Includes} ${Classes})

add_custom_target(
  RunSimulation ALL
  COMMAND Sim
)


