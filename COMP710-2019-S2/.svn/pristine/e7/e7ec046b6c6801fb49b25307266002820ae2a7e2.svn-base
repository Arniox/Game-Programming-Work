#include <iostream>
#include <ctime>
#include <string>
#include "HeaderEx2.h";

#define nl std::endl;
#define cls system("CLS");
#define wait cin.ignore();
#define clr cin.clear();

using namespace std;

//Door struct
struct Door {
	bool winning;
	int id;
	string outPut;
};

//Door array
Door doors[3];
bool playAgain = true;

int main() {
	srand(time(0));

	while (playAgain == true) {
		//Begin
		cout << "The Monty Hall Game Show!" << nl;
		cout << "-=-=-=-=-=-=-=-=-=-=-=-=-" << nl;
		cout << "PRESS ENTER TO CONTINUE" << nl;
		wait;
		cls;

		//Create winning door number
		int winningDoor = (rand() % 3) + 1;
		cout << "DEBUG - Winning door: " << winningDoor << nl;

		//Create pannels
		generateDoors(winningDoor);
		generateOutPutText();

		cout << "Which door does the player choose (1/2/3)? ";
		int userAnswer = takeInUserAnswer();
		int aiAnswer = simulateAI(userAnswer, winningDoor);
		bool switchChoose = getSwitch(userAnswer, aiAnswer);
		displayChosenDoors(userAnswer, switchChoose);
		checkIfUserWon(userAnswer, winningDoor);

		//Play again?
		checkPlayAgain();

	}
}

//Generate doors array
void generateDoors(int winningDoor) {
	//Fill all door data
	for (int i = 0; i < 3; i++) {
		doors[i].winning = false;
		doors[i].id = (i + 1);
		doors[i].outPut = to_string(doors[i].id);
	}

	//Set winning door data
	doors[winningDoor - 1].winning = true;
}

//Generate output text
void generateOutPutText() {
	string outPut = "";
	setTitles(outPut);
	setDoor(outPut);

	cout << outPut;
}

void setTitles(string& outPut) {
	outPut += "Door1  Door2  Door3  \n";
}

void setDoor(string& outPut) {
	outPut += "+---+  +---+  +---+  \n";
	outPut += "|   |  |   |  |   |  \n";
	outPut += "| " + doors[0].outPut;
	outPut += " |  ";
	outPut += "| " + doors[1].outPut;
	outPut += " |  ";
	outPut += "| " + doors[2].outPut;
	outPut += " |  \n";
	outPut += "|   |  |   |  |   |  \n";
	outPut += "+---+  +---+  +---+  \n";
}

//Take in and check user answer
int takeInUserAnswer() {
	int userAnswer;
	cin >> userAnswer;

	//Check
	if (userAnswer > 4 && userAnswer < 0) {
		cout << "This answer is not allowed. Please try again" << nl;
	}

	doors[userAnswer - 1].outPut = "x";

	return userAnswer;
}

//Simulate AI
int simulateAI(int userAnswer, int winningDoor) {
	int aiAnswer;

	//Pick not the user answer or winning door
	for (int i = 1; i < 4; i++) {
		if (i != userAnswer && i != winningDoor) {
			aiAnswer = i;
		}
	}

	cls;
	cout << "The game host chooses door number " << aiAnswer << nl;
	doors[aiAnswer - 1].outPut = "x";
	generateOutPutText();

	return aiAnswer;
}

//Get boolean
bool getSwitch(int& userAnswer, int aiAnswer) {
	int outPutDoor;
	for (int i = 1; i < 4; i++) {
		if (i != userAnswer && i != aiAnswer) {
			outPutDoor = i;
		}
	}

	cout << "Does the player want to switch from door " << userAnswer << " to door " << outPutDoor << " (y/n)? ";
	char userSwitch;
	cin >> userSwitch;

	if (userSwitch == 'y' || userSwitch == 'Y') {
		doors[userAnswer - 1].outPut = to_string(userAnswer);
		userAnswer = outPutDoor;
		return true;
	}
	else {
		return false;
	}
}

void displayChosenDoors(int userAnswer, bool switchUser) {
	//Check and output dynamic text
	if (switchUser == false) {
		cout << "Player chooses to stay with door " << userAnswer << "..." << nl;
	}
	else if (switchUser == true) {
		cout << "Player swaps to door " << userAnswer << "..." << nl;
	}

	doors[userAnswer - 1].outPut = "$";

	//Reveal answers
	generateOutPutText();
}

//Check if user won
void checkIfUserWon(int userAnswer, int winningAnswer) {
	if (userAnswer == winningAnswer) {
		cout << "Congratulations! The player wins!" << nl;
	}
	else {
		cout << "Oh no! The player loses!" << nl;
	}
}

void checkPlayAgain() {
	cout << "Play again (y/n)? " << nl;
	char in;
	cin >> in;

	if (in == 'y' || in == 'Y') {
		playAgain = true;
	}
	else {
		playAgain = false;
	}
}