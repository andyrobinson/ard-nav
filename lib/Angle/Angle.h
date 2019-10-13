#ifndef Angle_h
#define Angle_h

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
    angle to_angle(uangle ua);
}

#endif
