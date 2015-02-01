#include "LibDec.h"

cScores g_scores;

/////////////////////////////////////////////////
cScores::cScores()
{
}

/////////////////////////////////////////////////
cScores::~cScores()
{
}

/////////////////////////////////////////////////
bool cScores::Init(ifstream& fin)
{
	if (!fin)  
	{
		ErrorMsg("Scores.INIT: no fin found");
		return false;
	}

	if (DEBUG_MODE)
		debug << "CSCORE.INIT" << endl;
	//read data
	fin >> allTimeHighName >> allTimeHigh;
	currentScore = pointsToAdd = thisPlayHigh = 0;

	/////
	//calculate rectangle sizes needed
	/////
	currentRect.left = 0;
	g_graphics.DrawText("scoreFont", "CURRENT SCORE", 
						 currentRect, DT_CALCRECT | DT_SINGLELINE, 
						 255, 255, 255);
	currentRect.left += 5;
	currentRect.top += 5;
	currentRect.bottom += 5 + 24;
	currentRect.right += 5;

/*	currentHighRect.left = 0;
	g_graphics.DrawTextA("scoreFont", "YOUR BEST", 
						 currentHighRect, DT_CALCRECT | DT_SINGLELINE, 
						 255, 255, 255);
	currentHighRect.left += (g_app.GetWindowWidth() / 2) - (currentHighRect.right / 2);
	currentHighRect.top += 5;
	currentHighRect.bottom += 5 + 24;
	currentHighRect.right += currentHighRect.left;*/
	
	allTimeHighRect.left = 0;
	g_graphics.DrawText("scoreFont", "HIGH SCORE", 
						 allTimeHighRect, DT_CALCRECT | DT_SINGLELINE, 
						 255, 255, 255);
	allTimeHighRect.left += g_app.GetWindowWidth() - (allTimeHighRect.right * 2) - 5;
	allTimeHighRect.top += 5;
	allTimeHighRect.bottom += 5 + 24;
	allTimeHighRect.right += g_app.GetWindowWidth() - 5;

	if (DEBUG_MODE)
	{
		stringstream ss;
		ss << currentRect.left << ", " << currentRect.right << endl;
		debug << ss.str();
	}

	return true;
}
	
/////////////////////////////////////////////////
void cScores::AddToScore(int score)
{
	pointsToAdd += score;
}
	
/////////////////////////////////////////////////
void cScores::UpdateScore()
{
	/*int loopCounter = 0;
	int toAdd = 0;
	int pointsToAddCopy = pointsToAdd;
	while (pointsToAddCopy > 0)
	{
		toAdd += 10 * loopCounter;
		if (loopCounter == 0)
			toAdd += 1;

		pointsToAddCopy %= 10;
		loopCounter ++;
	}

	pointsToAdd -= toAdd;
	currentScore += toAdd;*/
	currentScore += pointsToAdd;
	if (NewCurrentHigh())
		thisPlayHigh = currentScore;
	pointsToAdd = 0;
}
	
/////////////////////////////////////////////////
void cScores::DisplayScores()
{
	stringstream ss;
	ss << "CURRENT SCORE\n" << currentScore;
	g_graphics.DrawText("scoreFont", ss.str().c_str(), 
						 currentRect, DT_CENTER, 
						 255, 255, 255);

	ss.str(std::string());
	if (NewBestHigh())
		ss << "HIGH SCORE\n" << thisPlayHigh;
	else
		ss << "HIGH SCORE\n" << allTimeHigh;
	g_graphics.DrawText("scoreFont", ss.str().c_str(), 
						 allTimeHighRect, DT_CENTER, 
						 255, 255, 255);

/*	ss.str(std::string());
	if (CheckBestHigh())
		ss << "HIGH SCORE\n" << thisPlayHigh;
	else
		ss << "HIGH SCORE\n" << allTimeHigh;
	g_graphics.DrawTextA("scoreFont", ss.str().c_str(), 
						 allTimeHighRect, DT_CENTER, 
						 255, 255, 255);*/
}

/////////////////////////////////////////////////
void cScores::ResetScore()
{
	thisPlayHigh = currentScore;
	currentScore = pointsToAdd = 0;
}

/////////////////////////////////////////////////
bool cScores::NewBestHigh()
{
	if (thisPlayHigh > allTimeHigh)
		return true;
	return false;
}

/////////////////////////////////////////////////
bool cScores::NewCurrentHigh()
{
	if (currentScore > thisPlayHigh)
		return true;
	return false;
}

/////////////////////////////////////////////////
void cScores::SaveNewHighScore()
{
	if (NewBestHigh())
	{
		const string& scoreFile = "HighScores.dat";
		ofstream scoreOut(scoreFile.c_str());
		scoreOut << "AAA " << thisPlayHigh;
		scoreOut.close();
	}
}