#ifndef Utility_h
#define Utility_h

namespace Utility {
  template <typename T> T sign(T value) {
   return T((value>0)-(value<0));
  }

  template <typename T> T abs1(T value) {
   return value < 0 ? -value : value;
  }

  long unix_time(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t second) {
    long years_since_1970 = year + 30; // year is years since 2000
    long leap_years_before_this = (years_since_1970 + 1) / 4;
    long year_days = (years_since_1970) * 365 + (leap_years_before_this);
    long month_days = 0;
    bool is_leap_year = (year % 4 == 0);

    switch (month-1) {
      case 11: month_days = month_days + 30;
      case 10: month_days = month_days + 31;
      case 9: month_days = month_days + 30;
      case 8: month_days = month_days + 31;
      case 7: month_days = month_days + 31;
      case 6: month_days = month_days + 30;
      case 5: month_days = month_days + 31;
      case 4: month_days = month_days + 30;
      case 3: month_days = month_days + 31;
      case 2: month_days = is_leap_year ? month_days + 29 : month_days + 28;
      case 1: month_days = month_days + 31;
    }

    // have to subtract1 because we're not zero based
    return ((year_days + month_days + day - 1) * 86400) + (hour * 3600) + (min * 60) + second;
  }


}

#endif
