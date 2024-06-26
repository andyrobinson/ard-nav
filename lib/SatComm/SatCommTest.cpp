#include "SatComm.h"
#include "gtest/gtest.h"
#include "time.h"
#include "Gps.h"
#include "StubLogger.h"
#include "Position.h"
#include "Battery.h"
#include "Compass.h"
#include "time.h"

using namespace Position;

namespace {

IridiumSBD stub_modem;
Timer stub_timer;
StubLogger stub_logger;
Gps stub_gps;
Battery stub_battery;
Compass stub_compass;

SatComm satcomm(&stub_modem, &stub_timer, &stub_gps, &stub_battery, &stub_compass, &stub_logger);

class SatCommTest : public ::testing::Test {
 protected:
  SatCommTest() {}
  void SetUp() override {}

  void initStubs(uangle bearing, int hour, int mins) {
    stub_modem.reset();
    stub_timer.reset();

    gpsResult gps_data = {{53.44580, -2.22515, 3.0},1,1.0,1.1,180,15000l,5344580,-222515};
    stub_gps.set_data(&gps_data,1);
    struct tm test_time = {0,mins,hour,2,3,123,5,6};
    stub_timer.setTime(mktime(&test_time));
    stub_timer.set_millis(20000);
    stub_compass.set_bearings(&bearing,1);

  }

  int32_t extract32(int offset, unsigned char *bin_data) {
     int32_t value = bin_data[offset+3] << 24 | bin_data[offset+2] << 16 | bin_data[offset+1] << 8 | bin_data[offset];
     return value;
  }

  uint16_t extractu16(int offset, unsigned char *bin_data) {
     uint16_t value = bin_data[offset+1] << 8 | bin_data[offset];
     return value;
  }

  time_t extractTime(int offset, unsigned char *bin_data) {
    return (time_t) extract32(offset, bin_data);
  }

  time_t extractLogTime(unsigned char *bin_data) {
    return extractTime(0,bin_data);
  }

  void extractLatLong(position *pos, unsigned char *bin_data) {
    int32_t la,lo;
    la = extract32(4,bin_data);
    lo = extract32(8,bin_data);
    pos->latitude = ((double)la)/100000.0;
    pos->longitude = ((double)lo)/100000.0;
  }

  float extractSog(unsigned char *bin_data) {
    int offset = 12;
    long value = bin_data[offset+3] << 24 | bin_data[offset+2] << 16 | bin_data[offset+1] << 8 | bin_data[offset];
    return ((float) value)/1000.0; // note approximation hack
  }

  unsigned short extractCog(unsigned char *bin_data) {
    int offset=16;
    unsigned short value = bin_data[offset+1] << 8 | bin_data[offset];
    return value;
  }

  void extractWpLabel(char& label, unsigned char *bin_data) {
        label = bin_data[18];
  }

  void extractBattery(uint16_t *max1, uint16_t *min1, uint16_t *max2, uint16_t *min2, unsigned char *bin_data) {
    *max1 = extractu16(19, bin_data);
    *min1 = extractu16(21, bin_data);
    *max2 = extractu16(23, bin_data);
    *min2 = extractu16(25, bin_data);
  }

  void extractCompass(unsigned short *bearing, uint8_t *errors, unsigned char *bin_data) {
    *bearing = extractu16(27, bin_data);
    *errors = bin_data[29];
  }

  time_t extractRestart(unsigned char *bin_data) {
    return extractTime(34,bin_data);
  }

};

TEST_F(SatCommTest, should_sleep_on_begin) {
    satcomm.begin();
    EXPECT_TRUE(stub_modem.isAsleep());
}

TEST_F(SatCommTest, steer_log_should_return_true_and_sleep_without_sending_if_not_logging_hour) {
    initStubs(100,4,0); // hour (10) not a multiple of 3
    satcomm.begin();

    bool result = satcomm.steer_log_or_continue();

    EXPECT_TRUE(result);
    EXPECT_EQ(stub_modem.send_attempts,0);
    EXPECT_TRUE(stub_modem.isAsleep());
}

TEST_F(SatCommTest, steer_log_should_reset_retry_interval_if_not_sending) {
    initStubs(100,4,0); // hour (10) not a multiple of 3
    satcomm.begin();
    EXPECT_EQ(stub_modem.retry_reset_count,0);

    bool result = satcomm.steer_log_or_continue();

    EXPECT_TRUE(result);
    EXPECT_EQ(stub_modem.send_attempts,0);
    EXPECT_EQ(stub_modem.retry_reset_count,1);
}

TEST_F(SatCommTest, steer_log_should_return_true_without_sending_if_not_logging_minute_window) {
    initStubs(120,12,26); // hour (12) a multiple of 3, but not within 5 mins of the hour
    satcomm.begin();

    bool result = satcomm.steer_log_or_continue();

    EXPECT_TRUE(result);
    EXPECT_EQ(stub_modem.send_attempts,0);
}

TEST_F(SatCommTest, steer_log_should_return_true_without_sending_if_logged_within_modem_retry_interval) {
    initStubs(120,6,0);
    stub_timer.set_millis(20000);
    satcomm.begin();
    stub_modem.set_response(ISBD_CANCELLED);

    bool result = satcomm.steer_log_or_continue();
    EXPECT_FALSE(result);
    EXPECT_EQ(stub_modem.send_attempts,1);

    stub_modem.sbdixInterval = 13; // seconds
    stub_timer.set_millis(32000); // 12 seconds later
    result = satcomm.steer_log_or_continue();

    EXPECT_TRUE(result);
    EXPECT_EQ(stub_modem.send_attempts,1);
}

TEST_F(SatCommTest, steer_log_should_send_if_within_logging_window) {
    initStubs(120,6,0); // hour (6) a multiple of 3, and mins start of the hour
    satcomm.begin();

    bool result = satcomm.steer_log_or_continue();

    EXPECT_TRUE(result);
    EXPECT_GT(stub_modem.send_attempts,0);
    EXPECT_EQ(stub_modem.retry_reset_count,0);
}

TEST_F(SatCommTest, steer_log_should_send_if_after_zero_but_within_window) {
    initStubs(120,18,4); // hour (18) a multiple of 3, and within 5 mins start of the hour
    satcomm.begin();

    bool result = satcomm.steer_log_or_continue();

    EXPECT_TRUE(result);
    EXPECT_GT(stub_modem.send_attempts,0);
}

TEST_F(SatCommTest, steer_log_should_send_the_correct_data_in_binary) {
    stub_modem.reset();
    stub_timer.reset();
    stub_timer.set_millis(20000);

    satcomm.begin();
    position result_pos;
    char result_wp_label;
    uint16_t result_batt_max1, result_batt_min1, result_batt_max2, result_batt_min2, result_bearing;
    uint8_t result_errors;

    // ** Setup **
    // time
    time_t first_time_set = 150890; // should be recorded as restart time
    stub_timer.setTime(first_time_set);
    struct tm test_time = {0,4,18,3,4,123,5,6};
    stub_timer.setTime(mktime(&test_time));

    // Gps data
    gpsResult gps_data = {{53.44580, -2.22515, 3.0},1,1.0,1.1,190,15000l,5344580,-222515};
    stub_gps.set_data(&gps_data,1);

    // waypoint
    char wp_label='Z';
    satcomm.set_dest(wp_label);

    // battery
    stub_battery.setMaxMin(0, 638,527);
    stub_battery.setMaxMin(1, 720,782);
  

    // compass
    uangle bearing = 163;
    stub_compass.set_err_percent(13);
    stub_compass.set_bearings(&bearing,1);

    // **** Execute ****
    satcomm.steer_log_or_continue();

    // ** Verify **
    EXPECT_EQ(stub_modem.sent_length,SAT_LOG_RECORD_SIZE);

    // time
    time_t logtime = extractLogTime(stub_modem.sent);
    EXPECT_EQ(logtime, stub_timer.now());

    // gps data
    extractLatLong(&result_pos,stub_modem.sent);
    float result_sog = extractSog(stub_modem.sent);
    unsigned short result_cog = extractCog(stub_modem.sent);
    EXPECT_NEAR(result_pos.latitude,gps_data.pos.latitude,0.000001);
    EXPECT_NEAR(result_pos.longitude,gps_data.pos.longitude,0.000001);
    EXPECT_NEAR(result_sog,gps_data.avg_mps,0.001);
    EXPECT_EQ(result_cog,gps_data.cog);

    // next waypoint label
    extractWpLabel(result_wp_label,stub_modem.sent);
    EXPECT_EQ(result_wp_label,wp_label);

    // battery
    extractBattery(&result_batt_max1,&result_batt_min1, &result_batt_max2,&result_batt_min2, stub_modem.sent);
    EXPECT_EQ(result_batt_max1, 638);
    EXPECT_EQ(result_batt_min1, 527);
    EXPECT_EQ(result_batt_max2, 720);
    EXPECT_EQ(result_batt_min2, 782);

    //compass
    extractCompass(&result_bearing,&result_errors, stub_modem.sent);
    EXPECT_EQ(result_bearing, 163);
    EXPECT_EQ(result_errors, 13);

    // Free memory - cannot test

    // - last restart (unix timestamp, stored in Timer)
    time_t result_restart = extractRestart(stub_modem.sent);
    EXPECT_EQ(result_restart,first_time_set);
}

TEST_F(SatCommTest, steer_log_should_retry_if_no_success_and_still_within_window) {
    initStubs(221,9,0);
    stub_modem.set_response(ISBD_SENDRECEIVE_TIMEOUT);
    satcomm.begin();

    satcomm.steer_log_or_continue();
    EXPECT_EQ(stub_modem.send_attempts,1);

    // wait a little but not too much
    stub_timer.set_millis (120000);

    satcomm.steer_log_or_continue();
    EXPECT_EQ(stub_modem.send_attempts,2);
}

TEST_F(SatCommTest, steer_log_should_abandon_if_no_sucess_and_past_window) {
    initStubs(221,9,0);
    stub_modem.set_response(ISBD_SENDRECEIVE_TIMEOUT);

    satcomm.begin();
    satcomm.steer_log_or_continue();
    EXPECT_EQ(stub_modem.send_attempts,1);

    // wait past end of window
    stub_timer.set_millis (500000);

    satcomm.steer_log_or_continue();
    EXPECT_EQ(stub_modem.send_attempts,1);
}

TEST_F(SatCommTest, steer_log_should_not_send_again_after_success_in_window) {
    initStubs(221,9,0);

    stub_modem.set_response(ISBD_SUCCESS);

    satcomm.begin();
    satcomm.steer_log_or_continue();
    EXPECT_EQ(stub_modem.send_attempts,1);

    // wait a little but not too much
    stub_timer.set_millis (120000);

    satcomm.steer_log_or_continue();
    EXPECT_EQ(stub_modem.send_attempts,1);
}

TEST_F(SatCommTest, steer_log_should_send_again_when_the_next_window_is_reached) {
    initStubs(305,9,0);

    stub_timer.set_millis(30000);
    stub_modem.set_response(ISBD_SUCCESS);

    satcomm.begin();
    satcomm.steer_log_or_continue();
    EXPECT_EQ(stub_modem.send_attempts,1);

    // wait three hours until next window
    stub_timer.set_millis (3 * 60 * SAT_MILLIS_IN_MINUTE);
    tm test_time = {0,0,12,2,3,123,5,6};
    stub_timer.setTime(mktime(&test_time));

    satcomm.steer_log_or_continue();
    EXPECT_EQ(stub_modem.send_attempts,2);
}

TEST_F(SatCommTest, steer_log_should_return_false_if_cancelled_by_callback) {
    initStubs(33,18,0);

    stub_modem.set_response(ISBD_CANCELLED);

    satcomm.begin();
    bool result = satcomm.steer_log_or_continue();
    EXPECT_EQ(stub_modem.send_attempts,1);
    EXPECT_FALSE(result);
}

TEST_F(SatCommTest, steer_log_should_not_log_if_no_time_available) {
    stub_modem.reset();
    stub_timer.reset();

    satcomm.begin();
    bool result = satcomm.steer_log_or_continue();

    EXPECT_EQ(stub_modem.send_attempts,0);
    EXPECT_TRUE(result);
}

TEST_F(SatCommTest, steer_log_should_use_satellite_time_if_no_time_available_and_have_waited) {
    initStubs(33,18,0);
    time_t test_time = 1686258005; // 8 June 2023 21.00
    tm modem_time;
    stub_timer.reset();
    stub_timer.set_millis (SAT_TIME_SET_INTERVAL_MINS * SAT_MILLIS_IN_MINUTE + 10);
    stub_modem.set_time(test_time - (SAT_TIME_SET_INTERVAL_MINS * 60));

    satcomm.begin();
    bool result = satcomm.steer_log_or_continue();

    EXPECT_EQ(stub_timer.now(), test_time);
    EXPECT_TRUE(stub_timer.isTimeSet());
    EXPECT_EQ(stub_modem.send_attempts,1);
    EXPECT_TRUE(result);
}

TEST_F(SatCommTest, steer_log_should_not_repeatedly_query_the_satellite_for_time) {
    initStubs(33,15,0);
    time_t test_time = 1686258005; // 8 June 2023 21.00
    tm modem_time;
    stub_timer.reset();
    stub_timer.set_millis (SAT_TIME_SET_INTERVAL_MINS * SAT_MILLIS_IN_MINUTE + 10);
    stub_modem.set_time(test_time - (SAT_TIME_SET_INTERVAL_MINS * 60));
    stub_modem.set_response(ISBD_NO_MODEM_DETECTED);

    satcomm.begin();
    satcomm.steer_log_or_continue();
    EXPECT_FALSE(stub_timer.isTimeSet());

    stub_modem.set_response(ISBD_SUCCESS);
    stub_timer.set_millis (stub_timer.milliseconds() + 10000);
    satcomm.steer_log_or_continue();

    EXPECT_FALSE(stub_timer.isTimeSet()); // we didn't try again

}

TEST_F(SatCommTest, steer_log_should_sleep_after_success) {
   initStubs(305,9,0);

    stub_modem.set_response(ISBD_SUCCESS);

    satcomm.begin();
    satcomm.steer_log_or_continue();
    EXPECT_EQ(stub_modem.send_attempts,1);
    EXPECT_TRUE(stub_modem.isAsleep());
}


TEST_F(SatCommTest, steer_log_should_not_sleep_after_failure) {
    initStubs(305,9,0);

    stub_modem.set_response(ISBD_SENDRECEIVE_TIMEOUT);

    satcomm.begin();
    satcomm.steer_log_or_continue();
    EXPECT_EQ(stub_modem.send_attempts,1);
    EXPECT_FALSE(stub_modem.isAsleep());
}

TEST_F(SatCommTest, steer_log_should_not_attempt_to_log_if_begin_fails) {
   initStubs(305,9,0);

    stub_modem.set_response(ISBD_SUCCESS);
    stub_modem.set_begin_response(ISBD_PROTOCOL_ERROR);

    satcomm.begin();
    satcomm.steer_log_or_continue();
    EXPECT_EQ(stub_modem.send_attempts,0);
    EXPECT_FALSE(stub_modem.isAsleep());
}

TEST_F(SatCommTest, steer_log_should_adjust_send_timeout_at_end_of_window) {
   initStubs(305,9,4);

    stub_modem.set_response(ISBD_SUCCESS);

    satcomm.begin();
    satcomm.steer_log_or_continue();
 
    EXPECT_EQ(stub_modem.send_attempts,1);
    EXPECT_EQ(stub_modem.send_timeout,60);

}

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
