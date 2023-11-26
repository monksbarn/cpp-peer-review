#include <string>
#include <stdexcept>
#include <array>

using namespace std;

struct DateTime {
   int year;
   int month;
   int day;
   int hour;
   int minute;
   int second;
};

///////////////////////////////////////////////////////////////////////////

void CheckDateTimeValidity(const DateTime& dt) {
   if (dt.year < 1 || dt.year > 9999) {
      throw domain_error("Year must be in the range from 1 to 9999"s);
   }

   if (dt.month < 1 || dt.month > 12) {
      throw domain_error("Month must be in the range from 1 to 12"s);
   }

   const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
   const array month_lengths = { 31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

   if (dt.day < 1 || dt.day > month_lengths[dt.month - 1]) {
      throw domain_error("Day must be in the range from 1 to"s + to_string(month_lengths[dt.month - 1]));
   }

   if (dt.hour < 0 || dt.hour > 23) {
      throw domain_error("Hour must be in the range from 0 to 23"s);
   }

   if (dt.minute < 0 || dt.minute > 59) {
      throw domain_error("Minute must be in the range from 0 to 59"s);
   }

   if (dt.second < 0 || dt.second > 59) {
      throw domain_error("Second must be in the range from 0 to 59");
   }
}