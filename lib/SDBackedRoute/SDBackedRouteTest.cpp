#include "SDBackedRoute.h"
#include "gtest/gtest.h"
#include "SD.h"

namespace {

SDBackedRoute route;

class SDBAckedRouteTest : public ::testing::Test {
 protected:
  SDBAckedRouteTest() {
  }

  void SetUp() override {

  }

};

TEST_F(SDBAckedRouteTest, Should_compile) {
    EXPECT_EQ(1, 2);
}


}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
