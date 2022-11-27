#include "Globe.h"
#include "gtest/gtest.h"
#include "math.h"

namespace {

Globe globe;

position London = {51.5073509,-0.12775823,0.0};
position Manchester = {53.479324,-2.2484851,0.0};
position Chorlton = {53.4407973,-2.272291,0.0};
position PlattFields = {53.44580, -2.22515,0.0};
position NewYork = {40.7127837, -74.0059413,0.0};
position Moscow = {55.755826, 37.6173,0.0};
position Sydney = {-33.8674869, 151.2069902,0.0};
position Capetown = {-33.9248685, 18.4240553,0.0};
position Santiago = {-33.4691199,-70.641997,0.0};

class GlobeTest : public ::testing::Test {
 protected:
  GlobeTest() {
    //sail = Sail(stub_servo);
     // You can do set-up work for each test here.
  }

  ~GlobeTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Objects declared here can be used by all tests in the test suite for Foo.
  double percentage_diff(double x, double y) {
    double diff = (100.0 * (x-y))/x;
    return fabs(diff);
  }
};

TEST_F(GlobeTest, should_calculate_distance_to_within_one_tenth_percent) {
  ASSERT_LT(percentage_diff(4565.0,globe.distance_between(&Manchester,&Chorlton)),0.1);
  ASSERT_LT(percentage_diff(262100.0,globe.distance_between(&Manchester,&London)),0.1);
  ASSERT_LT(percentage_diff(5570000.0,globe.distance_between(&London,&NewYork)),0.1);
  ASSERT_LT(percentage_diff(2543000.0,globe.distance_between(&Manchester,&Moscow)),0.1);
  ASSERT_LT(percentage_diff(11010000.0,globe.distance_between(&Sydney,&Capetown)),0.1);
  ASSERT_LT(percentage_diff(12560000.0,globe.distance_between(&Capetown,&NewYork)),0.1);
  ASSERT_LT(percentage_diff(11680000.0,globe.distance_between(&Santiago,&Chorlton)),0.1);
}

TEST_F(GlobeTest, should_calculate_bearing_between_points) {
  EXPECT_EQ(200,globe.bearing(&Manchester,&Chorlton));
  EXPECT_EQ(146,globe.bearing(&Manchester,&London));
  EXPECT_EQ(288,globe.bearing(&London,&NewYork));
  EXPECT_EQ(64,globe.bearing(&London,&Moscow));
  EXPECT_EQ(357,globe.bearing(&Santiago,&NewYork));
  EXPECT_EQ(80,globe.bearing(&Chorlton,&PlattFields));
  EXPECT_EQ(260,globe.bearing(&PlattFields,&Chorlton));
}

TEST_F(GlobeTest, should_calculate_new_position_from_bearing_and_distance) {
  position chorlton = globe.new_position(&Manchester,200,4565);
  position new_york = globe.new_position(&London,288,5570000);

  ASSERT_LT(percentage_diff(chorlton.latitude, Chorlton.latitude),1);
  ASSERT_LT(percentage_diff(chorlton.longitude, Chorlton.longitude),1);
  ASSERT_LT(percentage_diff(new_york.longitude, NewYork.longitude),1);
  ASSERT_LT(percentage_diff(new_york.latitude, NewYork.latitude),1);
}

TEST_F(GlobeTest, should_calculate_small_distances) {
  position moved = globe.new_position(&Manchester,200,0.9);
  double distance = globe.distance_between(&Manchester, &moved);

  ASSERT_LT(distance, 0.90001);
  ASSERT_GT(distance, 0.89999);
}

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
