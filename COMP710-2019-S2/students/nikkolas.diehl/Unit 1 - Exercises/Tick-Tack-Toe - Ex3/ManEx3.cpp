#include <iostream>
#include <ctime>
#include <string>
#include "HeaderEx3.h"

using namespace std;

#define nl std::endl
#define cls system("CLS")
#define wait cin.ignore()
#define clr cin.clear()
#define out std::cout
#define in std::cin
#define lst << nl

//Set up human players
struct Player {
	bool playing = false;
	char letter;
};

//set up cell
struct Cell {
	char item = ' ';
	string xWin = "==X==";
	string oWin = "==O==";
};
Cell gameArray[3][3];

//Proto-types
void initPlayers(Player players[2]);
void initRound(Player players[2]);
void gameRound(Player players[2]);

int main() {
	//rand
	srand((int)time(0));

	Player players[2];
	bool playing = true;
	while (playing) {
		initPlayers(players);
		initRound(players);
		gameRound(players);
	}

}

void initPlayers(Player players[2]) {
	players[0].letter = 'X';
	players[1].letter = 'O';
}

//Init round
void initRound(Player players[2]) {
	//intro
	out << "Hello and welcome to TicTac Toe! Press enter at any stop to continue" lst;
	wait;

	//Init the players
	switch (rand() % 2) {
	case 0:
		players[0].playing = true;
		players[0].playing = false;
		break;
	case 1:
		players[0].playing = false;
		players[0].playing = true;
		break;
	}
}

void printTable() {
	string outPut = "";
	outPut += "     |     |     |";
	outPut += "     |     |     |";
}

void gameRound(Player players[2]) {
	out << "Current player is: " << (players[0].playing ? "player 1" : "player 2") << nl;
	wait;
	out << "Please choose an X position"
}

//Check diagonal line
bool checkDiagLine() {
	return false;
}

//Check horizontal line
bool checkHorizLine() {
	return false;
}

//Check vertical line
bool checkVertLine() {
	return false;
}