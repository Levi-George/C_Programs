/*
Author: Levi George
Program Name: Sorry
Date Last Updated: 05/07/22
Program Purpose: This program will start a game of sorry, it will allow up to four players, and will follow the classic rules of a sorry game.
*/

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>

using namespace std;//I will be removing this

inline int rollDice();//This function will roll the dice
int checkRoll(int, int, int);//This function will check the roll, and return the code appropriate for the roll.
void Move7(int, int, int, int);//This function houses the move algorithm for the special rolls.
void Move(int, int, int, int);//This function houses the move algorithm for the normal rolls.
void printLocate(int, int, int);//This function will print the location of the players

//I might refactor the board representation to be a 4 space vector or array
//this will reduce the heavy lifting of looping through a 51 space vector
int playerPositions[4] = {0, 0, 0, 0}; 


int main()
{
	srand((unsigned(time(NULL))));//Seeds the random number generator

	int die1, die2;//These will store the die roll values

	int numOfPlayers;//This will store the number of players

	int turnCounter = 0;//This will keep track of whose turn it is
	int	rollCode;//This will represent the code assigned to the roll the player made
	int playerPos;//This will represent the current players position
	bool noWin = true;//This represents whether or not someone has won
	
	//This gets information from the users.
	cout << "Welcome to Sorry!" << '\n' << "How many players will there be today?(Max 4, Min 2)" << endl;
	cin >> numOfPlayers;

	//This loop will run to ensure the user doesn't put in any funky input
	while (!cin || numOfPlayers > 4 || numOfPlayers < 2)
	{
		cout << "I'm sorry, I can not set up a game of sorry with the value you put in, please enter an appropriate value." << endl;
		cin.clear();
		cin.ignore(1000, '\n');
		cin >> numOfPlayers;
	}
	
	//This makes up for the disparity with the array in counting - ~2018
	/*cout << "\nRemember, you will start on space 0, so if you were to roll a six on the first space, it will show you on space 5 at the end of your turn.\n";*/
	// ^^^^^^^ Anything in the backend of your systems should not be made transparent to the user, the system should work without exposing itself to the user. - 2022

	//This loop will run the actual portion which is the game, while no one has won
	while (noWin)
	{
		//This will print out the beginning of a player's turn
		cout << " \nIt is Player " << turnCounter << "'s turn" << endl;
		cout << "They are rolling the die. \n Rolling... (Press any button)" << endl;
		system("Pause");
		die1 = rollDice(); die2 = rollDice();
		cout << die1 << " " << die2 << "\n" << endl;

		//This will execute the necessary functions
		rollCode = checkRoll(die1, die2, turnCounter);

		//This will execute specific actions based on the roll code, and position of the player
		if ((rollCode == 7 || rollCode == 11) && playerPositions[turnCounter] != -1)
		{
			Move7(turnCounter, playerPositions[turnCounter], rollCode, numOfPlayers);
		}
		else if (rollCode == 12)//this roll code should always put the player at the zeroeth position since 
		{											//it is a double roll - remove  && playerPos != -1

			for(int i = 0; i < numOfPlayers; i++)
			{
				if(playerPositions[i] == 0)//in case a player was just put on spot 0
				{
					std::cout << "Player " << playerPositions[i] << "has been taken off the board, Player " << turnCounter << " is now at the start of the board." << endl;
					playerPositions[i] = -1;
				}
			}

			playerPositions[turnCounter] = 0;
			
		}
		else
		{
			Move(turnCounter, playerPos, rollCode, numOfPlayers);
		}

		//This will print the locations of the players
		printLocate(rollCode, turnCounter, numOfPlayers);

		//This will check if any player has won.
		for(int i = 0; i < numOfPlayers; i++)
		{
			if (playerPositions[i] == 50)
			{
				//This will allow the user to exit the game if they wish, as well as let them know they won.
				noWin = false;
				cout << "Player " << playerPositions[i] << " has reached the finish." << endl;
				numOfPlayers = 0;

				//This will reset the board
				for(int i = 0; i < 4; i++)
				{
					playerPositions[i] == 0;
				}

			}
		}

		//This will query the user as to whether or not they want to play again.
		if (!noWin)
		{
			//This will request the input and store it in pa (player acceptance).
			string pa = "N";//player acceptance

			cout << "Would you like to play again(y/n)?" << endl;
			cin >> pa;


			while ((pa != "y" && pa != "Y") && (pa != "n" && pa != "N"))
			{
				cout << " I'm sorry, I do not understand, would you like to play(y/n)?" << endl;
				cin >> pa;
			}

			//This will make sure the game runs again, or stops if the user wishes so
			if (pa == "Y" || pa == "y")
			{
				noWin = true;
			}
			else
			{
				cout << "Thanks for playing!" << endl;
				noWin = false;
			}




			//This will get the number of players again
			while ((!cin || numOfPlayers > 4 || numOfPlayers < 2 )&& (pa == "Y" || pa == "y"))
			{
				cout << "Please re-enter the number of players." << endl;
				cin.clear();
				cin.ignore(1000, '\n');
				cin >> numOfPlayers;
			}
		}

		//This will increment the turns, so if player one went first, this will allow player two to go next.
		turnCounter++;
		if (turnCounter > numOfPlayers)
		{
			turnCounter = 1;
		}

	}

	std::system("Pause");
}

//This function will roll a single die
inline int rollDice() 
{
	return rand() % 6+1;
}

//This function will facillitate movement for the special rolls
void Move7(int player, int playerPos, int rollCode, int numOfPlayers)
{
	//This will declare the lead and last players
	int leadIndex = player;
	int lastIndex = player;

	int temp = -1;

	for(int i = 1; i < numOfPlayers; i++)
	{
		if(playerPositions[i] < playerPositions[lastIndex])
		{
			lastIndex = i;
		}

		if(playerPositions[i] > playerPositions[leadIndex])
		{
			leadIndex = i;
		}
	}

	//Depending on what is rolled this will switch two players positions, from lead player to last, or last to lead
	if (leadIndex != player && rollCode == 7 && playerPositions[player] != -1)//Player must not be the leader and must be on the board
	{
		cout << "Player " << player << " switched places with Player " << leadIndex << endl;
		temp = playerPositions[player];
		playerPositions[player] = playerPositions[leadIndex];
		playerPositions[leadIndex] = temp;
	}
	else if (leadIndex == player && rollCode == 7 && playerPositions[player] != -1)//If player is in lead and they rolled a 7 (player swaps with lead) nothing changes
	{
		cout << " Nothing happened, Player " << player << " is already in the lead." << endl;
	}

	if (lastIndex != player && rollCode == 11 && playerPositions[player] != -1)//If the player isn't last and they are on the board, then they will get put in the last position.
	{
		cout << "Player " << player << " switched places with Player " << lastIndex << endl;
		temp = playerPositions[player];
		playerPositions[player] = playerPositions[lastIndex];
		playerPositions[lastIndex] = temp;
	}
	else if (lastIndex == player && rollCode == 11 && playerPositions[player] != -1)//If the player is in last or not on the board, then nothing will change.
	{
		cout << "Nothing happened. Player " << player << " is already in last." << endl;
	}


}


//This function will facilitate movement for the normal rolls
void Move(int player, int playerPos, int rollCode, int numOfPlayers)//player is the current player; playerPos is the players position on the board (array); numOfPlayers is the number of players.
{

	//This ensures they don't roll over 50 or under 0
	//if the player does not land exactly on 50, they do not win
	if ((playerPositions[player] + rollCode) < 0)
	{
		cout << "Player " << player << ": You have rolled a backspace, yet can go no further back, you are still on spot 0\n";
		playerPositions[player] = 0;
		return;
	}
	else if(playerPositions[player] + rollCode > 50)
	{
		cout << "Player " << player << "You have passed the end of the board, you are now at spot 0\n";
		playerPositions[player] = 0;
	}
	else
	{
		for(int i = 0; i < numOfPlayers; i++)
		{
			if(playerPositions[player] == playerPositions[i] && player != i)
			{
				cout << "Player " << i << ": You have been landed on by Player " << player << "\n";
				cout << "Player " << i << ": You have been set off the board, SORRY!\n";
			}
		}

		playerPositions[player] = playerPositions[player] + rollCode;
		cout << "Player " << player << ": You have moved to position " << playerPositions[player] << " \n";
	}

}

//This will classify the roll, and return a roll code, which will be used to move the player
int checkRoll(int die1, int die2, int players)//X is the first die, and Y is the second, players is the player currently rolling
{
	bool onBoard = false;//Keeps track of whether or not they are on the board
	bool amIonBoard = false;
	int z = 0;//Simple counter
	
	//This loop will check for any players on the board, and set the onBoard flag to true if there are people on the board
	for (int i = 0; i < 4; i++)
	{
		if (playerPositions[i] != -1)
		{
			onBoard = true;
			if(i == players)
				amIonBoard = true;
		}

	}

	//This will run, if and only if the player is not on the board, and they roll a double
	if (die1 == die2 && onBoard == false)
	{
		cout << "Player " << players << " has rolled a double, they can now move." << endl;
		onBoard = true;
	}

	//If onBoard is flagged as true, it will take the die sum(x+y) and translate it into a roll code
	if (onBoard == true)
	{
		cout << '\n';

		switch (die1 + die2)
		{
		case 2:
			cout << "Player " << players << " can move 2 spaces." << endl;
			return 2;
			break;
		case 3:
			cout << "Player " << players << " can move 3 spaces." << endl;
			return 3;
			break;
		case 4:
			cout << "Player " << players << " must move back 1 space." << endl;
			return -1;
			break;
		case 5:
			cout << "Player " << players << " can move 5 spaces." << endl;
			return 5;
			break;
		case 6:
			cout << "Player " << players << " can move 6 spaces." << endl;
			return 6;
			break;
		case 7:
			cout << "Player " << players << " can switch with the leader." << endl;
			return 7;
			break;
		case 8:
			cout << "Player " << players << " can move 8 spaces." << endl;
			return 8;
			break;
		case 9:
			cout << "Player " << players << " can move 9 spaces." << endl;
			return 9;
			break;
		case 10:
			cout << "Player " << players << " can move 10 spaces." << endl;
			return 10;
			break;
		case 11:
			cout << "Player " << players << " has to switch places with the player in last." << endl;
			return 11;
			break;
		case 12:
			cout << "Player " << players << " has to start from the beginning." << endl;
			return 12;
			break;
		}
	}
	else//If they are not on the board, it will display this error message
	{
		cout << "Player " << players << " can not yet move, Player " << players << " has not rolled a double." << endl;
		return 0;
	}
	
	//user should not reach this point
	std::cout << "Player has befuddled the game :[";
	return 0;
}

//Refactored for using four index array that tracks locations - LEG - 22/12/11
//printLocate will find/print the locations of each player
void printLocate(int x, int players, int numOfPlayers)
{

	for (int i = 0; i < numOfPlayers; i++)
	{
		
		if (playerPositions[i] == -1)//If the player is not on the board, and the loop is finished, this will tell the user
		{
			cout << "Player " << i << " is not on the board yet." << endl;
		}
		else //If the player is on the board, this will tell the user where
		{
			cout << "Player " << i << " is at position " << playerPositions[i] << endl;
		}

	}
}