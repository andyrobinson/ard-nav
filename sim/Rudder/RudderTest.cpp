#include "Rudder.h"
#include "StubLogger.h"
#include "gtest/gtest.h"

namespace {

position chorlton = {53.44096, -2.27235, 5.0};
StubLogger logger;
Boat boat(&chorlton, &logger);
Rudder rudder(&boat);

class RudderTest : public ::testing::Test {
 protected:
  RudderTest() {}
};

TEST_F(RudderTest, should_set_rudder_on_boat) {
  rudder.set_position(23);
  EXPECT_EQ(boat.rudder, 23);
}

TEST_F(RudderTest, Should_not_move_rudder_to_right_beyond_limits) {
  rudder.set_position(-50);
  EXPECT_EQ(boat.rudder, -RUDDER_MAX_DISPLACEMENT);
}

TEST_F(RudderTest, Should_not_move_rudder_to_left_beyond_limits) {
  rudder.set_position(60);
  EXPECT_EQ(boat.rudder, RUDDER_MAX_DISPLACEMENT);
}

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
