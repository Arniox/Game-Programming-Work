#pragma once
#include <string>

using namespace std;

//function prototypes
void generateDoors(int winningDoor);
void generateOutPutText();
void setTitles(string& outPut);
void setDoor(string& outPut);
int takeInUserAnswer();
int simulateAI(int userAnswer, int winningDoor);
bool getSwitch(int& userAnswer, int aiAnswer);
void displayChosenDoors(int userAnswer, bool switchUser);
void checkIfUserWon(int userAnswer, int winningAnswer);
void checkPlayAgain();