#include "SDBackedRoute.h"
#include "gtest/gtest.h"
#include "SD.h"

using namespace SDLib;

namespace {

SDBackedRoute broute(&SDStub);
waypoint wpa = {"A",   {53.43553, -2.27068, 5}};
waypoint wpb = {"B",   {53.43519, -2.27118, 5}};
waypoint wpc = {"C",   {53.43533, -2.27141, 5}};
waypoint wpd = {"D",   {53.43544, -2.27176, 5}};
waypoint wpaarr[] = {wpa};
waypoint wpa2[] = {wpa,wpb};
waypoint wpa4[] = {wpa,wpb,wpc,wpd};

class SDBAckedRouteTest : public ::testing::Test {
 protected:
  SDBAckedRouteTest() {
  }
  void SetUp() override {
  }

};

TEST_F(SDBAckedRouteTest, Should_write_a_single_char_to_a_file) {
    char contents[10];
    File f = File(contents);
    f.write('a');
    EXPECT_EQ(f.contents_as_string()[0],'a');
}

TEST_F(SDBAckedRouteTest, Should_write_a_single_char_to_a_file_and_return_as_a_string) {
    char contents[10];
    File f = File(contents);
    f.write('b');
    EXPECT_STREQ(f.contents_as_string(),"b");
}

TEST_F(SDBAckedRouteTest, Should_write_a_sequence_of_bytes_and_return_as_a_string) {
    uint8_t thing[6] = "thing";
    char contents[10];
    File f = File(contents);
    f.write(thing, 5);
    EXPECT_STREQ(f.contents_as_string(),"thing");
}

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
    SDStub.reset();
    char filename[] = "thing.dat";
    char file_contents[] = "0001";
    SDStub.setup_file(filename,file_contents);
    route onewaypoint={"thing",1,wpaarr};
    broute.begin(&onewaypoint,true);
    EXPECT_STREQ(SDStub.last_remove(),"thing.dat");
}

TEST_F(SDBAckedRouteTest, Should_not_delete_the_route_file_on_warm_start) {
    SDStub.reset();
    char filename[] = "thing.dat";
    char file_contents[] = "0001";
    SDStub.setup_file(filename,file_contents);
    route onewaypoint={"thing",1,wpaarr};
    broute.begin(&onewaypoint,false);
    EXPECT_STREQ(SDStub.last_remove(),"");
}

TEST_F(SDBAckedRouteTest, Should_pick_up_a_previous_waypoint_from_the_file_on_warm_start) {
    char filename[] = "blah.dat";
    char file_contents[] = "000102";
    SDStub.reset();
    SDStub.setup_file(filename,file_contents);
    route four={"blah",4,wpa4};
    broute.begin(&four,false);
    waypoint *resume = broute.next();
    EXPECT_EQ(resume->label[0], wpc.label[0]);
}

TEST_F(SDBAckedRouteTest, Should_start_from_waypoint_zero_if_no_file_exists) {
    SDStub.reset();
    route four={"blah",4,wpa4};
    broute.begin(&four,false);
    waypoint *resume = broute.next();
    EXPECT_EQ(resume->label[0], wpa.label[0]);
}

TEST_F(SDBAckedRouteTest, Should_append_a_two_digit_index_to_the_route_file_on_next) {
    SDStub.reset();
    route four={"nexttest",4,wpa4};
    broute.begin(&four,true);
    broute.next();
    broute.next();
    EXPECT_STREQ(SDStub.file_contents(),"0001");

}
// TEST_F(SDBAckedRouteTest, Should_constrain_route_index_even_if_file_is_nonsense) {}

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
