#ifndef Angle_h
#define Angle_h

#define DEGREES_IN_RADIAN 57.2958
#define ANGLE_ERROR 399

namespace Angle {
    typedef short angle;
    typedef unsigned short uangle;

    angle clockwise(angle start, uangle delta);
    angle anticlock(angle start, uangle delta);
    angle add(angle start, angle delta);
    uangle to_uangle(angle a);

    uangle uclockwise(uangle start, uangle delta);
    uangle uanticlock(uangle start, uangle delta);
    uangle uadd(uangle start, angle delta);
    angle udiff(uangle a, uangle b);
    angle to_angle(uangle ua);
    uangle  clockwisediff(uangle start, uangle end);

    bool in_range(uangle value, uangle lower, uangle upper);

    double to_radians(double degrees);
    double to_degrees(double radians);
}

#endif
