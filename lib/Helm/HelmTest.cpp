#include "StubLogger.h"
#include "Helm.h"
#include "Rudder.h"
#include "RotaryPID.h"
#include "gtest/gtest.h"
#include "Utility.h"

using namespace Utility;

namespace {

Rudder stub_rudder;
Compass stub_compass;
Timer stub_timer;
WindSensor stub_windsensor;
Sail stub_sail;
RotaryPID stub_rotaryPID;
Helm helm;
StubLogger logger;
SatComm stub_satcomm;

angle wind_sample[] = {180};

class HelmTest : public ::testing::Test {
 protected:
  HelmTest() {
  }

  void SetUp() override {
    stub_rudder.reset();
    stub_sail.reset();
    stub_windsensor.set_relative(wind_sample, 1);
    helm = Helm(&stub_rudder, &stub_compass, &stub_timer, &stub_windsensor, &stub_sail, &stub_rotaryPID, &stub_satcomm, &logger);
  }

  angle rudder_position() {
    return *stub_rudder.get_positions();
  }
};

TEST_F(HelmTest, Stub_rudder_should_record_last_position) {
  stub_rudder.set_position(27);
  EXPECT_EQ(rudder_position(), 27);
}

TEST_F(HelmTest, Stub_compass_should_return_bearing_set) {
  uangle bearing = 99;
  stub_compass.set_bearings(&bearing,1);
  EXPECT_EQ(stub_compass.bearing(), 99);
}

TEST_F(HelmTest, stub_windsensor_should_return_readings_sticking_on_last) {
  angle wind_values[] = {4,5};
  stub_windsensor.set_relative(wind_values,2);
  EXPECT_EQ(stub_windsensor.relative(),4);
  EXPECT_EQ(stub_windsensor.relative(),5);
  EXPECT_EQ(stub_windsensor.relative(),5);
}

TEST_F(HelmTest, Should_steer_using_the_value_from_the_rotary_pid_calculator) {
  uangle bearing = 0;
  angle result[] = {-10};
  stub_compass.set_bearings(&bearing,1);
  stub_rotaryPID.set_results(result,1);
  helm.steer(30, 1, {65, 355});
  EXPECT_EQ(rudder_position(), -10);
}

 TEST_F(HelmTest, Should_steer_repeatedly_given_overall_time_and_wait_time) {
   uangle bearings[] = {0, 3, 8};
   angle pid_results[] = {-10,-23,-34};
   stub_compass.set_bearings(bearings, 3);
   stub_rotaryPID.set_results(pid_results,3);
   helm.steer(30, 1500, {65,355});

   angle *positions = stub_rudder.get_positions();
   EXPECT_EQ(positions[0],pid_results[0]);
   EXPECT_EQ(positions[1],pid_results[1]);
   EXPECT_EQ(positions[2],pid_results[2]);
 }

TEST_F(HelmTest, Should_set_the_sail_every_time_we_steer) {
  uangle bearings[] = {210,190};
  angle wind[] = {70, 70, 90, 90};
  angle pid_results[] = {-5,-20};
  stub_compass.set_bearings(bearings, 2);
  stub_windsensor.set_relative(wind, 4);
  stub_rotaryPID.set_results(pid_results,2);

  helm.steer(200, 750, {235,165});

  angle *positions = stub_rudder.get_positions();
  angle *sail_calls = stub_sail.get_calls();

  EXPECT_EQ(positions[0],pid_results[0]);
  EXPECT_EQ(positions[1],pid_results[1]);
  EXPECT_EQ(sail_calls[0],wind[1]);
  EXPECT_EQ(sail_calls[1],wind[3]);
}

TEST_F(HelmTest, Should_abort_the_tack_if_the_wind_is_repeatedly_out_of_range) {
  uangle bearing = 0;
  angle result[] = {-10};
  angle wind_sample[] = {50};
  stub_windsensor.set_relative(wind_sample, 1);
  stub_compass.set_bearings(&bearing,1);
  stub_rotaryPID.set_results(result,1);

  helm.steer(30, (MAX_OUT_OF_RANGE+1) * STEER_INTERVAL, {65, 355});

  EXPECT_STREQ(logger.last_message(), "Abandon:  50| 65|355");
}

TEST_F(HelmTest, Should_not_abort_the_tack_before_max_out_range_exceeded) {
  uangle bearing = 0;
  angle result[] = {-10};
  angle wind_sample[] = {50};
  stub_windsensor.set_relative(wind_sample, 1);
  stub_compass.set_bearings(&bearing,1);
  stub_rotaryPID.set_results(result,1);

  helm.steer(30, (MAX_OUT_OF_RANGE-1) * STEER_INTERVAL, {65, 355});

  EXPECT_STRNE(logger.last_message(), "Abandon:  50| 65|355");
}

 TEST_F(HelmTest, Should_steer_straight_passing_sail_error_value_if_wind_sensor_in_error) {
   uangle bearings[] = {0, 3, 8};
   angle pid_results[] = {-10,-23,-34};
   angle wind_sample[] = {ANGLE_ERROR};

   stub_windsensor.set_relative(wind_sample, 1);
   stub_compass.set_bearings(bearings, 3);
   stub_rotaryPID.set_results(pid_results,3);
   helm.steer(30, 1500, {65,355});

   angle *positions = stub_rudder.get_positions();
   angle *sail_calls = stub_sail.get_calls();

   EXPECT_EQ(positions[0],pid_results[0]);
   EXPECT_EQ(positions[1],pid_results[1]);
   EXPECT_EQ(positions[2],pid_results[2]);

   EXPECT_EQ(sail_calls[0],ANGLE_ERROR); // Sail will deal with this OK and set to 30
   EXPECT_EQ(sail_calls[1],ANGLE_ERROR);
   EXPECT_EQ(sail_calls[2],ANGLE_ERROR);
 }

// TODO: Use GPS value instead here
 TEST_F(HelmTest, Should_steer_straight_ignoring_PID_and_passing_sail_wind_value_if_compass_in_error) {
   uangle bearings[] = {ANGLE_ERROR};
   angle pid_results[] = {-10,-23,-34};
   angle wind_sample[] = {70,70,70,70,90,90};

   stub_windsensor.set_relative(wind_sample, 6);
   stub_compass.set_bearings(bearings, 1);
   stub_rotaryPID.set_results(pid_results,3);
   helm.steer(30, 1500, {65,355});

   angle *positions = stub_rudder.get_positions();
   angle *sail_calls = stub_sail.get_calls();

   EXPECT_EQ(positions[0],0);
   EXPECT_EQ(positions[1],0);
   EXPECT_EQ(positions[2],0);

   EXPECT_EQ(sail_calls[0],70);
   EXPECT_EQ(sail_calls[1],70);
   EXPECT_EQ(sail_calls[2],90);
 }

  TEST_F(HelmTest, should_call_satcomm_steer_and_continue_on_true) {
    uangle bearing = 0;
    angle result[] = {-10};
    stub_compass.set_bearings(&bearing,1);
    stub_rotaryPID.set_results(result,1);

    helm.steer(30, 1, {65, 355});

    EXPECT_EQ(rudder_position(), -10);
    EXPECT_EQ(stub_satcomm.steer_count,1);
  }


// TEST_F(HelmTest, should_call_satcomm_steer_and_finish_on_false) {}


}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
