#include "SatComm.h"
#include "gtest/gtest.h"
#include "time.h"

namespace {

IridiumSBD stub_modem;
Timer stub_timer;
SatComm satcomm(&stub_modem, &stub_timer);

class SatCommTest : public ::testing::Test {
 protected:
  SatCommTest() {}
  void SetUp() override {}
};

TEST_F(SatCommTest, should_sleep_on_begin) {
    satcomm.begin();
    EXPECT_TRUE(stub_modem.isAsleep());
}

TEST_F(SatCommTest, steer_log_should_return_true_without_sending_if_not_logging_hour) {
    satcomm.begin();
    struct tm test_time = {0,0,10,2,3,123,5,6}; // hour (10) not a multiple of 3
    // printf("** Test date: %s", asctime(&test_time));
    stub_timer.setTime(mktime(&test_time));
    bool result = satcomm.steer_log_or_continue();

    EXPECT_TRUE(result);
    EXPECT_EQ(stub_modem.send_attempts,0);
}

TEST_F(SatCommTest, steer_log_should_return_true_without_sending_if_not_logging_minute_window) {
    satcomm.begin();
    struct tm test_time = {0,30,12,2,3,123,5,6}; // hour (12) a multiple of 3, but not within 5 mins of the hour
    // printf("** Test date: %s", asctime(&test_time));
    stub_timer.setTime(mktime(&test_time));
    bool result = satcomm.steer_log_or_continue();

    EXPECT_TRUE(result);
    EXPECT_EQ(stub_modem.send_attempts,0);
}

TEST_F(SatCommTest, steer_log_should_send_if_within_logging_window) {
    stub_modem.reset();
    satcomm.begin();

    struct tm test_time = {0,0,12,2,3,123,5,6}; // hour (12) a multiple of 3, but not within 5 mins of the hour
    //printf("** Test date: %s", asctime(&test_time));
    stub_timer.setTime(mktime(&test_time));
    bool result = satcomm.steer_log_or_continue();

    EXPECT_TRUE(result);
    EXPECT_GT(stub_modem.send_attempts,0);
}

TEST_F(SatCommTest, steer_log_should_send_if_after_zero_but_within_window) {
    stub_modem.reset();
    satcomm.begin();
    struct tm test_time = {0,4,15,2,3,123,5,6}; // hour a multiple of 3, within 5 mins of the hour
    //printf("** Test date: %s", asctime(&test_time));
    stub_timer.setTime(mktime(&test_time));
    bool result = satcomm.steer_log_or_continue();

    EXPECT_TRUE(result);
    EXPECT_GT(stub_modem.send_attempts,0);
}

TEST_F(SatCommTest, steer_log_should_send_the_correct_data) {
    stub_modem.reset();
    satcomm.begin();
    struct tm test_time = {0,4,21,3,4,123,5,6};
    stub_timer.setTime(mktime(&test_time));
    printf("** Test date: %s", asctime(&test_time));

    bool result = satcomm.steer_log_or_continue();
    printf("**DATA** [%s]", stub_modem.sent);
    EXPECT_TRUE(result);
    EXPECT_GT(stub_modem.send_attempts,0);
}
//TEST_F(SatCommTest, steer_log_should_retry_if_no_success_and_still_within_window) {}
//TEST_F(SatCommTest, steer_log_should_abandon_if_no_sucess_and_past_window) {}
//TEST_F(SatCommTest, steer_log_should_not_send_again_after_success_in_window) {}
//TEST_F(SatCommTest, steer_log_should_send_again_when_the_next_window_is_reached) {}
//TEST_F(SatCommTest, steer_log_should_return_false_if_cancelled_by_callback) {}


}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
