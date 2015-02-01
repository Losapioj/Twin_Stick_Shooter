#include "LibDec.h"

cTimer::cTimer () : cntsPerSec (0), 
					prevTimeStamp (0)
{
	QueryPerformanceFrequency ((LARGE_INTEGER*)&cntsPerSec);
	secsPerCnt = 1.0f / (float)cntsPerSec;
	QueryPerformanceCounter ((LARGE_INTEGER*)&prevTimeStamp);
}

float cTimer::GetTime ()
{  
	currTimeStamp = 0;
	QueryPerformanceCounter ((LARGE_INTEGER*)&currTimeStamp);
	float dt = (currTimeStamp - prevTimeStamp) * secsPerCnt;
	prevTimeStamp = currTimeStamp;
	return dt;
}

float cTimer::getSecsPerCnt ()
{
	return secsPerCnt;
}

////////////////////////////////////////////////
cTotalGameTime::cTotalGameTime()
{
	milliseconds = 0.0;
	seconds = minutes = hours = days = 0;
}
cTotalGameTime::cTotalGameTime(float milli, int sec, int min, 
							   int hr, int day) : milliseconds(milli),
							   seconds(sec), minutes(min), hours(hr),
							   days(day)
{}
void cTotalGameTime::getGameTime(float* milli, int* sec, int* min, 
						         int* hr, int* day)
{
	*milli = milliseconds;
	*sec = seconds;
	*min = minutes;
	*hr = hours;
	*day = days;
}

void cTotalGameTime::updateTime(float dt)
{
	milliseconds += dt * MILLI_IN_SEC;
	while (milliseconds >= MILLI_IN_SEC)
	{
		milliseconds -= MILLI_IN_SEC;
		seconds += 1;
//		debug << seconds << " ";
//		if (seconds % 30 == 0)
//			debug << "\nsec: ";
		if (seconds % SEC_BTWN_SPAWNS == 0)
		{
			if (!g_entities.SpawnEnemyByTime(ENEMY_ONE) && DEBUG_MODE)
			{
				ErrorMsg("GAMETIME TO SPAWN ENEMY: enemy spawn fail");
			}
		}
	}
	while (seconds >= SEC_IN_MIN)
	{
		seconds -= SEC_IN_MIN;
		minutes += 1;
//		debug << "min: " << minutes << "\n";
	}
	while (minutes >= MIN_IN_HR)
	{
		minutes -= MIN_IN_HR;
		hours += 1;
//		debug << "hr: " << hours << "\n";
	}
	while (hours >= HR_IN_DAY)
	{
		hours -= HR_IN_DAY;
		days += 1;
	}
}

float cTotalGameTime::getMilli(float* milli)
{
	if (milli == NULL)
		return milliseconds;
	*milli = milliseconds;
	return milliseconds;
}
int cTotalGameTime::getSec(int* sec)
{
	if (sec == NULL)
		return seconds;
	*sec = seconds;
	return seconds;
}
int cTotalGameTime::getMin(int* min)
{
	if (min == NULL)
		return minutes;
	*min = minutes;
	return minutes;
}
int cTotalGameTime::getHr(int* hr)
{
	if (hr == NULL)
		return hours;
	*hr = hours;
	return hours;
}
int cTotalGameTime::getDay(int* day)
{
	if (day == NULL)
		return days;
	*day = days;
	return days;
}

int cTotalGameTime::getTotalSec(int* sec)
{
	int total = days * HR_IN_DAY;
	total = (total + hours) * MIN_IN_HR;
	total = (total + minutes) * SEC_IN_MIN;
	total += seconds;
	if (sec == NULL)
		return total;
	*sec = total;
	return total;
}

void cTotalGameTime::setGameTime(float milli, int sec, int min, 
								 int hr, int day)
{
	if (DEBUG_MODE)
		debug << "GAME TIME RESET" << endl;
	milliseconds = milli;
	seconds = sec;
	minutes = min;
	hours = hr;
	days = day;
}