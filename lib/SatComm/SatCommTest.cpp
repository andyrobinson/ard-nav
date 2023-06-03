#include "SatComm.h"
#include "gtest/gtest.h"
#include "time.h"
#include "Gps.h"
#include "StubLogger.h"
#include "Position.h"
#include "Battery.h"
#include "Compass.h"

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

  int32_t extract32(int offset, unsigned char *bin_data) {
     int32_t value = bin_data[offset+3] << 24 | bin_data[offset+2] << 16 | bin_data[offset+1] << 8 | bin_data[offset];
     return value;
  }

  time_t extractTime(unsigned char *bin_data) {
    return (time_t) extract32(0, bin_data);
  }

  uint16_t extractu16(int offset, unsigned char *bin_data) {
     uint16_t value = bin_data[offset+1] << 8 | bin_data[offset];
     return value;
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

  void extractWpLabel(char *label, unsigned char *bin_data) {
    int offset=18;
    for (int i=0;i<2;i++) {
        label[i] = bin_data[i+offset];
    }
    label[2] = '\0';
  }

  void extractBattery(uint16_t *max, uint16_t *min, unsigned char *bin_data) {
    *max = extractu16(20, bin_data);
    *min = extractu16(22, bin_data);
  }
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
    gpsResult gps_data = {{53.44580, -2.22515, 3.0},1,1.0,1.1,170,15000l,5344580,-222515};
    stub_gps.set_data(&gps_data,1);

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
    gpsResult gps_data = {{53.44580, -2.22515, 3.0},1,1.0,1.1,180,15000l,5344580,-222515};
    stub_gps.set_data(&gps_data,1);
    struct tm test_time = {0,4,15,2,3,123,5,6}; // hour a multiple of 3, within 5 mins of the hour
    //printf("** Test date: %s", asctime(&test_time));
    stub_timer.setTime(mktime(&test_time));
    bool result = satcomm.steer_log_or_continue();

    EXPECT_TRUE(result);
    EXPECT_GT(stub_modem.send_attempts,0);
}

TEST_F(SatCommTest, steer_log_should_send_the_correct_data_in_binary) {
    stub_modem.reset();
    satcomm.begin();
    position result_pos;
    char result_wp_label[3];
    uint16_t result_batt_max, result_batt_min;

    // ** Setup **
    // time
    struct tm test_time = {0,4,21,3,4,123,5,6};
    stub_timer.setTime(mktime(&test_time));

    // Gps data
    gpsResult gps_data = {{53.44580, -2.22515, 3.0},1,1.0,1.1,190,15000l,5344580,-222515};
    stub_gps.set_data(&gps_data,1);

    // waypoint
    char wp_label[3]="A5";
    satcomm.set_dest(wp_label);

    // battery
    stub_battery.setMaxMin(638,527);

    // compass
    uangle bearing = 163;
    stub_compass.set_err_percent(13);
    stub_compass.set_bearings(&bearing,1);

    // **** Execute ****
    satcomm.steer_log_or_continue();

    // ** Verify **
    EXPECT_EQ(stub_modem.sent_length,SAT_LOG_RECORD_SIZE);

    // time
    time_t logtime = extractTime(stub_modem.sent);
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
    EXPECT_STREQ(result_wp_label,wp_label);

    // battery
    extractBattery(&result_batt_max,&result_batt_min, stub_modem.sent);
    EXPECT_EQ(result_batt_max, 638);
    EXPECT_EQ(result_batt_min, 527);

    //todo:
    // - Compass heading and I2C errors,
    // - Free memory
    // - last restart (unix timestamp, stored in Timer)
}
//TEST_F(SatCommTest, steer_log_should_retry_if_no_success_and_still_within_window) {}
//TEST_F(SatCommTest, steer_log_should_abandon_if_no_sucess_and_past_window) {}
//TEST_F(SatCommTest, steer_log_should_not_send_again_after_success_in_window) {}
//TEST_F(SatCommTest, steer_log_should_send_again_when_the_next_window_is_reached) {}
//TEST_F(SatCommTest, steer_log_should_return_false_if_cancelled_by_callback) {}
//TEST_F(SatCommTest, steer_log_should_use_satellite_time_if_no_time_available) {}
//TEST_F(SatCommTest, steer_log_should_not_log_if_no_time_or_satellite_time) {}

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
