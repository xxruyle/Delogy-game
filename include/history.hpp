#pragma once

enum MONTH { JANUARY, FEBRUARY, MARCH, APRIL };

namespace History {
extern int year;
extern int month;
extern int day;
extern int era;
extern float hour;
extern float lastTime;

extern const int yearsInEra;
extern const int daysInMonth;
extern const float hourInDay;

void update();

} // namespace History
