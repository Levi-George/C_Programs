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

int rollDice();//This function will roll the dice
int checkBoard(int, int, int);//This function will check the board for the location of the player
int checkRoll(int, int, int);//This function will check the roll, and return the code appropriate for the roll.
void Move7(int, int, int, int);//This function houses the move algorithm for the special rolls.
void Move(int, int, int, int);//This function houses the move algorithm for the normal rolls.
void printLocate(int, int, int);//This function will print the location of the players

//This array represents the board
int Board[51] = {};

//I might refactor the board representation to be a 4 space vector or array
//this will reduce the heavy lifting of looping through a 51 space vector

int main()
{
	srand((unsigned(time(NULL))));//Seeds the random number generator

	int die1, die2;//These will store the die roll values

	int players;//This will store the number of players

	int turnCounter = 1;//This will keep track of whose turn it is
	int	rollCode;//This will represent the code assigned to the roll the player made
	int playerPos;//This will represent the current players position
	bool noWin = true;//This represents whether or not someone has won
	
	//This gets information from the users.
	cout << "Welcome to Sorry!" << '\n' << "How many players will there be today?(Max 4, Min 2)" << endl;
	cin >> players;

	//This loop will run to ensure the user doesn't put in any funky input
	while (!cin || players > 4 || players < 2)
	{
		cout << "I'm sorry, I can not set up a game of sorry with the value you put in, please enter an appropriate value." << endl;
		cin.clear();
		cin.ignore(1000, '\n');
		cin >> players;
	}
	
	//This makes up for the disparity with the array in counting - ~2018
	/*cout << "\nRemember, you will start on space 0, so if you were to roll a six on the first space, it will show you on space 5 at the end of your turn.\n";*/

	// ^^^^^^^ Anything in the backend of your systems should not be made transparent to the user, the system should work without exposing itself to the user. - 2022

	//This loop will run the actual portion which is the game, while no one has won
	while (noWin)
	{
		//This will print out the beginning of a player's turn
		cout << " \nIt is Player " << turnCounter << "'s turn" << endl;
		cout << "They are rolling the die. \n Rolling..." << endl;
		system("Pause");
		die1 = rollDice(); die2 = rollDice();
		cout << die1 << " " << die2 << "\n" <<endl;

		//This will execute the necessary functions
		rollCode = checkRoll(die1, die2, turnCounter);
		playerPos = checkBoard(rollCode, turnCounter, players);

		//This will execute specific actions based on the roll code, and position of the player
		if ((rollCode == 7 || rollCode == 11) && playerPos != -1)
		{
			Move7(turnCounter, playerPos, rollCode, players);
		}
		else if (rollCode == 12)//this roll code should always put the player at the zeroeth position since 
		{											//it is a double roll - remove  && playerPos != -1

			if(!Board[0])//in case a player was just put on spot 0
			{
				std::cout << "Player " << Board[0] << "has been taken off the board, Player " << turnCounter << " is now at the start of the board." << endl;
			}

			Board[playerPos] = 0;
			Board[0] = turnCounter; //this doubles as showing the current player
		}
		else if(rollCode == 0)
		{
			//This will save some computing power by skipping Move when rollCode == 0
		}
		else
		{
			Move(turnCounter, playerPos, rollCode, players);
		}

		//This will print the locations of the players
		printLocate(rollCode, turnCounter, players);

		//This will check if any player has won.
		if (Board[50] != 0)
		{
			//This will allow the user to exit the game if they wish, as well as let them know they won.
			noWin = false;
			cout << "Player " << Board[50] << " has reached the finish." << endl;
			players = 0;

			//This will reset the board
			for (int i = 0; i < 51; i++)
			{
				Board[i] = 0;
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
			while ((!cin || players > 4 || players < 2 )&& (pa == "Y" || pa == "y"))
			{
				cout << "Please re-enter the number of players." << endl;
				cin.clear();
				cin.ignore(1000, '\n');
				cin >> players;
			}
		}

		//This will increment the turns, so if player one went first, this will allow player two to go next.
		turnCounter++;
		if (turnCounter > players)
		{
			turnCounter = 1;
		}

	}

	std::system("Pause");
}

//This function will roll a single die
int rollDice() 
{
	return rand() % 6+1;
}
//This function will facillitate movement for the special rolls
void Move7(int player, int playerPos, int rollCode, int numOfPlayers)
{
	//This will declare the lead and last players
	int leadIndex = playerPos;
	int leadPlayer = player;
	int lastIndex = playerPos;
	int lastPlayer = player;
	

	//*TODO: Fix this iterator, if we have four pre-determined user IDs we shouldn't have to do this, just iterate across the board
	//			and mark the last person we found.			*

	/*
		int curr = -1 -This will be used to keep track of last player found
		int playersFound = 0;

		for ( # of spaces in board)
			if(curr is -1 and board[i] != 0)
				lastPlayer = board[i]
				curr = board[i]

			else if(board[i] != 0)
				playersFound++;
			
			if(playersFound == #ofPlayers)
				leadPlayer == board[i]

	*/

	//This function will determine who is in the lead, and who is last by sorting them.
	for (int i = 1; i < numOfPlayers; i++)
	{
		for (int j = 0; j < 51; j++)
		{
			if (leadIndex < j && Board[j] != 0)
			{
				leadIndex = j;
				leadPlayer = Board[j];
			}
			if (lastIndex > j && Board[j] != 0)
			{
				lastIndex = j;
				lastPlayer = Board[j];
			}
		}
	}

	//Depending on what is rolled this will switch two players positions, from lead player to last, or last to lead
	if (rollCode == 7 && leadIndex != playerPos && playerPos != -1)
	{
		cout << "Player " << player << " switched places with Player " << leadPlayer << endl;
		Board[leadIndex] = player;
		Board[playerPos] = leadPlayer;
	}
	else if (leadIndex == playerPos && rollCode == 7 && playerPos != -1)
	{
		cout << " Nothing happened, Player " << player << " is already in the lead." << endl;
	}

	if (rollCode == 11 && lastIndex != playerPos && playerPos != -1)
	{
		cout << "Player " << player << " switched places with Player " << lastPlayer << endl;
		Board[lastIndex] = player;
		Board[playerPos] = lastPlayer;
	}
	else if (lastIndex == playerPos && rollCode == 11 && playerPos != -1)
	{
		cout << "Nothing happened. Player " << player << " is already in last." << endl;
	}


}


//This function will facilitate movement for the normal rolls
void Move(int player, int playerPos, int rollCode, int numOfPlayers)//player is the current player; playerPos is the players position on the board (array); numOfPlayers is the number of players.
{

	//This ensures they don't roll over 50 or under 0
	//if the player does not land exactly on 50, they do not win
	if (((playerPos + rollCode) > 50 || (playerPos + rollCode) < 0) && (rollCode != 12 && rollCode != 7 && rollCode != 11))
	{
		Board[playerPos] = 0;
		return;
	}

	if(Board[(playerPos+rollCode)])//we can say != 0 or since 0 is falsy we can drop the comparison
	{

		std::cout << "Player " << player << " has overtaken " << Board[playerPos+rollCode] << ". \n";

		//player at lead spot goes to players old spot
		Board[playerPos] = Board[playerPos+rollCode];

		Board[playerPos+rollCode] = player; //player moves to new position
	}
	else
	{
		Board[playerPos] = 0;				//player is no longer at old position
		Board[playerPos+rollCode] = player; //player moves to a new position
	}
}


//checkBoard will do the math, and actual sorting to figure out where the current player is.
int checkBoard(int x, int players, int numOfPlayers) //X is the sum of the two dies, players is the current player, and numOfPlayers is the number of players
{
	int pLocate = -1;//This will track the current player's position
	int y = 1;//This is a counter

	//This will locate the current player
	while (y <= numOfPlayers)
	{
		for (int i = 0; i < 51; i++)
		{
			if (Board[i] == players)
			{
				pLocate = i;
			}
			
		}
		y++;
	}
	
	//We don't need to return -1 if pLocate is negative one. -L 05/10/22
	//This will return a negative one, if they are on the start, or their location, if they are not
	/*
	if (pLocate == -1)
	{
		return -1;
	}
	else
		return pLocate;
	*/

	return pLocate;
}

//This will classify the roll, and return a roll code, which will be used to move the player
int checkRoll(int x, int y, int players)//X is the first die, and Y is the second, players is the player currently rolling
{
	bool onBoard = false;//Keeps track of whether or not they are on the board
	int z = 0;//Simple counter
	
	//This loop will check for any players on the board, and set the onBoard flag to true if there are people on the board
	for (int i = 0; i < 51; i++)
	{
		if (Board[i] == players)
		{
			onBoard = true;
		}
	}

	//This will run, if and only if the player is not on the board, and they roll a double
	if (x == y && onBoard == false)
	{
		cout << "Player " << players << " has rolled a double, they can now move." << endl;
		onBoard = true;
	}

	//Checks if they are on the board, if the counter has reached 50(whichs implies the user isn't on the board), and if the
	//two die aren't similar(if they are, this loop is pointless, because the player is now able to move.
	while (onBoard == false && z < 51 && x != y)	
	{
		if (Board[z] == players)
		{
			onBoard = true;
		}
		z++;
	}

	//If onBoard is flagged as true, it will take the die sum(x+y) and translate it into a roll code
	if (onBoard == true)
	{
		cout << '\n';

		switch (x + y)
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

//printLocate will find/print the locations of each player
void printLocate(int x, int players, int numOfPlayers)
{
	//These will keep track of the players it has tracked, and prevents double output
	int y = 1;
	int checkcounter = 0;

	//This will find and then tell the user where the players are.
	while (y <= numOfPlayers)
	{
		checkcounter = 0;//This will set the counter to 0, in order to prevent improper output
		for (int i = 0; i < 51; i++)
		{
			//If the player is on the board, this will tell the user where
			if (Board[i] == y)
			{
				cout << "Player " << y << " is at position " << (i) << endl;
				checkcounter++;
			}
			else if(i == 50 && checkcounter == 0)//If the player is not on the board, and the loop is finished, this will tell the user
			{
				cout << "Player " << y << " is not on the board yet." << endl;
			}

		}
		y++;
	}
}