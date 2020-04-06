#ifndef SelfTest_h
#define SelfTest_h

class SelfTest
{
  public:
    SelfTest();
    SelfTest(Gps *gpsp, WindSensor *windsensorp, Compass *compassp, Sail *sailp, Rudder *rudderp);
    void test();

  private:
    Gps *gps;
    Windsensor *windsensor;
    Compass *compass;
    Sail *sail;
    Rudder *rudder;
};

#endif
