#include "SatComm.h"
#include "gtest/gtest.h"

namespace {

IridiumSBD stub_modem;
SatComm satcomm(&stub_modem);

class SatCommTest : public ::testing::Test {
 protected:
  SatCommTest() {
  }
  void SetUp() override {

  }

};

TEST_F(SatCommTest, should_sleep_on_begin) {
    satcomm.begin();
    EXPECT_TRUE(stub_modem.isAsleep());
}

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
