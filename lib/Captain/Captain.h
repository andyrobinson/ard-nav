#ifndef Captain_h
#define Captain_h

#include "Position.h"
#include "Navigator.h"

using namespace Position;

class Captain
{
  public:
    Captain();
    Captain(Navigator *navigatorp);
    void voyage(position *route, int length);

  private:
    Navigator *navigator;
};

#endif
