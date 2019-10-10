#ifndef Sail_h
#define Sail_h

class Sail
{
  public:
    Sail();
    int sail_position(int relative_wind);

  private:
    int sign(int n);
    int abs(int n);
};

#endif
