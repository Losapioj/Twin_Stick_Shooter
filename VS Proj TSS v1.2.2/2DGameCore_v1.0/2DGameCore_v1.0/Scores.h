#ifndef __SCORES_H__
#define __SCORES_H__


/////////////////////////////////////////////////

class cScores
{
private:
	unsigned long currentScore;
	unsigned long pointsToAdd;

	unsigned long thisPlayHigh;
	unsigned long allTimeHigh;
	string allTimeHighName;

	RECT currentRect,/* currentHighRect,*/ allTimeHighRect;

public:
	cScores();
	~cScores();

	//reads .dat file
	bool Init(ifstream& fin);
	//adds amount to pointsToAdd
	void AddToScore(int score);
	//adds to score in parts to give score a dynamic feel
	void UpdateScore();
	//displays the scores in game
	void DisplayScores();
	//reset score to 0 when player dies, update high score if needed
	void ResetScore();

	//checks current vs best high score, return true for new best
	bool NewBestHigh();
	//saves new high score, if there is one
	void SaveNewHighScore();
	//checks current vs current high score, return true for new best
	bool NewCurrentHigh();
};

#endif __SCORES_H__