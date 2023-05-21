#include "SatComm.h"
#include "gtest/gtest.h"

namespace {

class SatCommTest : public ::testing::Test {
 protected:
  SatCommTest() {
  }
  void SetUp() override {
  }

};

TEST_F(SatCommTest, Should_so_something) {
    EXPECT_EQ(1,2);
}

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
