#ifndef Position_h
#define Position_h

namespace Position {

  typedef struct {
     double latitude;
     double longitude;
     double latError;
     double longError;
  } position;

}

#endif
