
// THE MOST FANTACTIC FANTASTICAL SNAIL RACE EVER IN HISTORY OF ALL TIME AND FOREVER!
//                                                                                    - AND EVER!

// the reference website:
// https://books.google.dk/books?id=wfwLAAAAQBAJ&pg=PT119&lpg=PT119&dq=c%2B%2B+snail+race&source=bl&ots=kfXtm4MJ1m&sig=hNLmXa9uM86XfvnwCu5zKYp0GtU&hl=da&sa=X&ved=0ahUKEwik1_-Ou5DXAhUOZ1AKHXu9C9QQ6AEIKjAB#v=onepage&q=c%2B%2B%20snail%20race&f=false


#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <string>

#define WINNING_POSITION 20

//int menu(void); // the menu players start the game in.

//Function for taking user input untill the player writes "ROLL" and then returns a string of the updated positions of the current cients' snail
std::string yourTurn(int); 

//Fucntion for returning a bool if any of the
bool findWinner(int); // declare the winner

//Simulates a six sided dice, thrown and returns its value.
int Dice(); // Dice function


int pos[3] = { 0,0,0 }; // the position of the snails - will store how far down the track the individual snails is.
int Snailpos;			// players snail position
int diceThrow;			//clobal enheritable variable for current dice throw

/*Currently not needed...*/
//int main() {
//
//	int userResponse;
//	cout << " Welcome to THE MOST FANTACTIC FANTASTICAL SNAIL RACE EVER IN HISTORY OF ALL TIME AND FOREVER!\n" << endl;
//	cout << "                                                                                                - AND EVER!!!!\n" << endl;
//
//	userResponse = menu(); {
//		switch (userResponse) {
//		case 1:
//			cout << "And the snails are off\n"
//				<< "Look at them GO!!!\n" << endl;
//			yourTurn();
//			break;
//		}
//
//	}
//	return 0; // end program!!
//
//}// end main

/*currently not needed...*/
//int menu(void) { //display the main menu and return the users selection
//
//	int userResponse;
//
//	{
//		cout << "Race Menu\n"
//
//			<< "\n" << "Are you ready to race? \n"
//			<< "1) Yes! GO! \n"
//			<< "2) SEE YOU LATER ALLIGATOR! (leave race) \n" << endl;
//		cin >> userResponse;
//	}
//
//	return userResponse;
//
//}//end menu function

std::string yourTurn(int i)
{
	printf("\nHey look! It is your turn, player %d!\n", (i));
	//_________________________________Roll dice
	std::string input;
	printf("roll the die\n\n");

	std::string checkRoll;
	while (checkRoll != "roll") {
		std::getline(std::cin, checkRoll);
	}

	diceThrow = Dice();
	pos[i] = pos[i] + diceThrow;

	printf("your new position is: %d\n\n", pos[i]);

	//return value to send to other players.
	return ("\nSnail" + std::to_string(i) + " proceeded with " + std::to_string(diceThrow) + " and is now on position " + std::to_string(Snailpos) + /*check for a winner snail(your snail)*/(Snailpos >= 20 ? "\n Snail " + std::to_string(i) + " wins!" : "\n") + /*Are you leading the pack?*/ (pos[i] > pos[i + 1 % 3] && pos[i] > pos[i + 2 % 3] ? "Snail" + std::to_string(i) + " is the leader of the pack!\n" : "\n"));
}

int Dice() {
	int result;
	for (int counter = 0; counter < 2; counter++) {
		srand(time(NULL));
		for (int i = 0; i < 6; i++) {
			result = rand() % 6 + 1;
			printf("\nDice: %d\n", result);
			return result;
		} result = 0;
	}
}

bool findWinner(int index) {
	if (pos[0] >= WINNING_POSITION) {
		if (index+48 == 0) {
			printf("\n\nYOU WIN!\n\n");
			return true;
		}
		printf("\n\nPLAYER0 WINS!");
		return true;
	}
	if (pos[1] >= WINNING_POSITION) {
		if (index == 1) {
			printf("\n\nYOU WIN!\n\n");
			return true;
		}
		printf("\n\nPLAYER1 WINS!");
		return true;
	}
	if (pos[2] >= WINNING_POSITION) {
		if (index == 2) {
			printf("\n\nYOU WIN!\n\n");
			return true;
		}
		printf("\n\nPLAYER2 WINS!");
		return true;
	}

	return false;
}

#pragma once
