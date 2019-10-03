#include <iostream>
#include <ctime>
#include <random>
#include <string>
#include "HeaderEx1.h"

#define nl std::endl;
#define cls system("CLS");
#define wait cin.ignore();
#define clr cin.clear();

using namespace std;

//Create players struct
struct Player {
	int diceRollOne;
	int diceRollTwo;
};

Player players[2];
bool playAgain;

//Win counter
float playerWon = 0;
float aiWon = 0;
float aiWinPercentage = 0;
float playerWinPercentage = 0;

int main() {
	srand(time(0));

	playAgain = true;

	int roundCount = 0;
	while (playAgain) {
		//Play game
		int computerIsCheatingValue = (rand() % 100) + 1;
		//Generate dice rolls for players
		GeneratePlayers();
		CheckCheating(computerIsCheatingValue);
		PlayRound(roundCount);

		cout << "Do you wish to play again? Type y to play again!" << nl;

		input();

		roundCount++;
	}

}

void GeneratePlayers() {
	//Generate four dice rolls
	int diceRolls[] = {
		((rand() % 6) + 1),
		((rand() % 6) + 1),
		((rand() % 6) + 1),
		((rand() % 6) + 1)
	};

	//Set dice rolls in alternating fashion
	players[0].diceRollOne = diceRolls[0];
	players[1].diceRollOne = diceRolls[1];
	players[0].diceRollTwo = diceRolls[2];
	players[1].diceRollTwo = diceRolls[3];
}

void CheckCheating(int computerIsCheatingValue) {
	if (computerIsCheatingValue > 0 && computerIsCheatingValue < 71) {
		//Computer is cheating
		players[1].diceRollTwo = players[1].diceRollOne;
	}
	else {
		players[0].diceRollTwo = players[0].diceRollOne;
	}
}

void PlayRound(int roundCount) {
	cout << "Hello and welcome to the dice rolling game. Press enter to continue at every stopping point!" << nl;
	wait;
	cls;
	cout << "Current win ratio: AI: " << aiWon << " PLAYER: " << playerWon << nl;
	if (!((aiWon + playerWon) == 0)) {
		aiWinPercentage = ((aiWon) / (aiWon + playerWon)) * 100;
		playerWinPercentage = ((playerWon) / (aiWon + playerWon)) * 100;
	}
	cout << "AI wins " << aiWinPercentage << "% of the time. The player wins " << playerWinPercentage << "% of the time" << nl;

	//Check which player starts
	if (roundCount % 2 == 0) {
		cout << "First player to start is: AI!" << nl;
		cout << "His first roll is: " << players[1].diceRollOne << nl;
		wait;
		cout << "Your first roll is: " << players[0].diceRollOne << nl;
		wait;
		cout << "His second roll is: " << players[1].diceRollTwo << nl;
		wait;
		cout << "Your second roll is: " << players[0].diceRollTwo << nl;
		wait;
	}
	else {
		cout << "First player to start is: YOU, the user!" << nl;
		cout << "Your first roll is: " << players[0].diceRollOne << nl;
		wait;
		cout << "His first roll is: " << players[1].diceRollOne << nl;
		wait;
		cout << "Your second roll is: " << players[0].diceRollTwo << nl;
		wait;
		cout << "His second roll is: " << players[1].diceRollTwo << nl;
		wait;
	}

	//Check who wins
	checkWhoWon();
}

void checkWhoWon() {
	//AI wins
	if (players[1].diceRollOne == players[1].diceRollTwo) {
		cout << "Sorry not sorry, the AI has won!!!" << nl;
		aiWon++;
	}//Tie
	else if ((players[1].diceRollOne == players[1].diceRollTwo) && (players[0].diceRollOne == players[0].diceRollTwo)) {
		cout << "IT WAS A TIE. Congradulations!" << nl;
		aiWon++;
		playerWon++;
	}//Player wins
	else if (players[0].diceRollOne == players[0].diceRollTwo) {
		cout << "Congradulations!!!! You have won!!!" << nl;
		playerWon++;
	}
	else {
		cout << "NO ONE WON. HA SHAME BITCH!" << nl;
	}

	//Play again?
	wait;
}

void input() {
	char input;
	clr;
	cin >> input;

	//Check which answer the user gave
	if (input == 'Y' || input == 'y') {
		playAgain = true;
	}
	else {
		playAgain = false;
	}
}

