#include "SDBackedRoute.h"
#include "gtest/gtest.h"
#include "SD.h"

using namespace SDLib;

namespace {

SDBackedRoute broute(&SDStub);
waypoint wpa = {"A",   {53.43553, -2.27068, 5}};
waypoint wpb = {"B",   {53.43519, -2.27118, 5}};
waypoint wpaarr[] = {wpa};
waypoint wpa2[] = {wpa,wpb};

class SDBAckedRouteTest : public ::testing::Test {
 protected:
  SDBAckedRouteTest() {
  }
  void SetUp() override {
  }

};

TEST_F(SDBAckedRouteTest, Should_return_done_for_an_empty_route) {
    route empty={"zero",0,{}};
    broute.begin(&empty,true);
    EXPECT_TRUE(broute.done());
}

TEST_F(SDBAckedRouteTest, Should_not_return_done_for_a_non_empty_route) {
    route onewaypoint={"one",1,wpaarr};
    broute.begin(&onewaypoint,true);
    EXPECT_FALSE(broute.done());
}

TEST_F(SDBAckedRouteTest, Should_return_done_once_we_have_iterated_through_the_route) {
    route onewaypoint={"one",1,wpaarr};
    broute.begin(&onewaypoint,true);
    EXPECT_FALSE(broute.done());
    broute.next();
    EXPECT_TRUE(broute.done());
}

TEST_F(SDBAckedRouteTest, Calling_next_returns_the_waypoints_in_order) {
    route twowaypoint={"two",2,wpa2};
    broute.begin(&twowaypoint,true);
    waypoint *wp1 = broute.next();
    waypoint *wp2 = broute.next();
    EXPECT_EQ(wp1,&(wpa2[0]));
    EXPECT_STREQ(wp1->label,"A");
    EXPECT_EQ(wp2,&(wpa2[1]));
    EXPECT_STREQ(wp2->label,"B");
}

TEST_F(SDBAckedRouteTest, Should_not_go_beyond_the_last_waypoint) {
    route onewaypoint={"one",1,wpaarr};
    broute.begin(&onewaypoint,true);
    broute.next();
    waypoint *wpend = broute.next();
    EXPECT_EQ(wpend,&(wpaarr[0]));
    EXPECT_STREQ(wpend->label,"A");
}

TEST_F(SDBAckedRouteTest, Should_delete_any_previous_file_on_cold_start) {
    route onewaypoint={"thing",1,wpaarr};
    broute.begin(&onewaypoint,true);
    EXPECT_STREQ(SDStub.last_remove(),"thing.dat");
}

TEST_F(SDBAckedRouteTest, Should_not_delete_the_route_file_on_warm_start) {
    SDStub.reset();
    route onewaypoint={"thing",1,wpaarr};
    broute.begin(&onewaypoint,false);
    EXPECT_STREQ(SDStub.last_remove(),"");
}

TEST_F(SDBAckedRouteTest, Should_write_a_two_digit_index_to_the_route_file) {
    SDStub.reset();
    route onewaypoint={"stuff",1,wpaarr};
    broute.begin(&onewaypoint,false);
    EXPECT_STREQ(SDStub.last_filepath(),"stuff.dat");
    EXPECT_STREQ(SDStub.file_contents(),"00");
}

// TEST_F(SDBAckedRouteTest, Should_pick_up_a_previous_waypoint_from_the_file_on_warm_start) {}
// TEST_F(SDBAckedRouteTest, Should_append_a_two_digit_index_to_the_route_file_on_next) {}




}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
