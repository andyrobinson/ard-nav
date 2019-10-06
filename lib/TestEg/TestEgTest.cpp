#include "TestEg.h"
#include "gtest/gtest.h"

namespace {

// The fixture for testing class Foo.
class TestEgTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  TestEgTest() {
     // You can do set-up work for each test here.
  }

  ~TestEgTest() override {
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

// Tests that the Foo::Bar() method does Abc.
TEST_F(TestEgTest, fAddsOne) {
  TestEg eg;
  EXPECT_EQ(eg.f(1),2);
}

// Tests that Foo does Xyz.
TEST_F(TestEgTest, fAddsTwo) {
  TestEg eg;
  EXPECT_EQ(eg.g(1),3);
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
