#ifndef Position_h
#define Position_h

namespace Position {

  typedef struct {
     double latitude;
     double longitude;
     double error;
  } position;

  inline bool operator==(const position& lhs, const position& rhs) {
      return (lhs.latitude == rhs.latitude &&
        lhs.longitude == rhs.longitude &&
        lhs.error == rhs.error);
  }
}
#endif
