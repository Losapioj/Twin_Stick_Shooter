// Timer.h - class cTimer definition

#ifndef __TIMER_H__
#define __TIMER_H__

#include <windows.h>

// gives change in time every instance through loop
class cTimer
{
private:
	__int64 cntsPerSec;
	float secsPerCnt;
	__int64 currTimeStamp,
			prevTimeStamp;

public:
	cTimer ();

	float GetTime ();
	float getSecsPerCnt ();
}; 

//saves total game time
class cTotalGameTime
{
private:
	float milliseconds;
	int seconds;
	int minutes;
	int hours;
	int days;

public:
	cTotalGameTime();
	cTotalGameTime(float milli, int sec, int min, int hr, int day);
	void getGameTime(float* milli, int* sec, int* min, int* hr, int* day);
	//sets game time, pass nothing to reset to 0
	void setGameTime(float milli = 0, int sec = 0, 
					 int min = 0, int hr = 0, 
					 int day = 0);

	// takes pointer and fills value
	// returns value as well to nest calls
	// allows no input for just return value.
	float getMilli(float* milli = NULL);
	int getSec(int* sec = NULL);
	int getMin(int* min = NULL);
	int getHr(int* hr = NULL);
	int getDay(int* day = NULL);

	int getTotalSec(int* sec = NULL);
	
	// takes dt and updates game time appropriately
	void updateTime(float dt);
	
};


/////////////////////////////////////////////////
#endif // __TIMER_H__