#ifndef Position_h
#define Position_h

namespace Position {

  typedef struct Position {
     double latitude;
     double longitude;
     double latError = 0.0;
     double longError = 0.0;
  };

}

#endif
