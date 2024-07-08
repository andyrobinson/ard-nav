set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CSS_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(${TestName} ${Sources})
target_include_directories(${TestName} PUBLIC  ${Includes})

target_link_libraries(
  ${TestName}
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${TestName})

add_custom_target(
  RunTests ALL
  COMMAND ${TestName}
)
