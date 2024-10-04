#include "history.hpp"
#include "raylib.h"

namespace History {
int year = 1;
int month = JANUARY;
int day = 1;
int era = 1;
float hour = 0.0f;
float lastTime = 0.0f;

const int yearsInEra = 10;
const int daysInMonth = 30;
const float hoursInDay = 5;
void update()
{
	if (GetTime() - History::lastTime > History::hoursInDay) {
		History::day += 1;
		History::hour = 0.0f;
		History::lastTime = GetTime();
	}
	else {
		History::hour = (GetTime() - History::lastTime);
	}

	if (History::day > daysInMonth) {
		History::month += 1;
		History::day = 1;
	}

	if (History::month > APRIL) {
		History::year += 1;
		History::month = JANUARY;
	}

	if (History::year > yearsInEra) {
		History::era += 1;
		History::year = 1;
	}
}

} // namespace History
