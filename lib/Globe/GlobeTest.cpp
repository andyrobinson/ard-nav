#include "Globe.h"
#include "gtest/gtest.h"

namespace {

Globe globe;

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

};

TEST_F(GlobeTest, Globe_should_do_something) {
  position test_pos = globe.new_position({2.0,2.0}, 56, 2.6);
  EXPECT_EQ(test_pos.latitude, 99.0);
}

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
