#ifndef Helm_h
#define Helm_h

#include "Rudder.h"
#include "Compass.h"

class Helm
{
  public:
    Helm();
    Helm(Rudder *rudder, Compass *compass);
    void steer(uangle direction);

  private:
    Rudder *rudder;
    Compass *compass;
    angle rudder_position;

};

#endif
