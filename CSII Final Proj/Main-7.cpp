/*
Program Name: I play too much red dead
Author: Levi George
Date Last Updaed: 5/11/2019
Program Purpose: To allow a user to play through several schemes to make money for their gang to retire
*/
#include <iostream>
#include <random>
#include <string>
#include <iomanip>
#include <Windows.h>
#include "Posse.h"
#include "Target.h"
#include "CattleRustling.h"
#include "Member.h"
#include "Bank Robbery.h"
#include "StageCoach.h"
#include <exception>

int main()
{
	//This values will be used for initialization of objects
	std::string GangName;
	std::string MemberName;
	int MemberClass;

	//Per Member; the total amount of money for each member to retire
	double PerMem = 2000;

	//Holds total number of heists
	std::vector<Target*> heists;

	//This vector and it's parts will be used for display purposes only
	std::vector<Target*> examples;
	examples.push_back(new Rustle());
	examples.push_back(new Coach());
	examples.push_back(new Bank());

	//Tracks current heist
	int currheist = 0;

	//random number seed
	srand(unsigned(time(NULL)));

	std::cout << std::fixed << std::setprecision(2);

	std::cout << "Basic Explanation: \nWin Condition: You earn enough money for each of your men to retire with $" << PerMem << std::endl;
	std::cout << "Lose Condition: You lose all of your gang members.\n";
	std::cout << "\nAs you decide to do a mission, keep in mind there is a certain point when, if the success rate is low enough, you can lose a gang member.\n";
	std::cout << "If the success rate is less than 100% you will earn less than the total amount of money possible.\n";

	system("pause");

	//gets gang name from user
	std::cout << "\nWe need to form your gang." << std::endl;
	std::cout << "What is your gang's name?" << std::endl;
	std::cin >> GangName;
	
	//Initializes Gang
	Posse Gang(GangName);
	
	//Begins the member creation process
	std::cout << "\nYour gang will have at most 3 members to begin and must always have at least 1. \nIf all of them get arrested you will fail.\n" << std::endl;
	while (Gang.size() < 3)
	{
		//gets recruit name
		std::cout << "Enter the name of member " << Gang.size() << "\n";
		std::cin.ignore(1000, '\n');
		getline(std::cin, MemberName);

		//gets recruit class value
		//From here on out, the use of class is meant to denoted a specific job that a member takes on, not a specific C++ class included with this 
		//program
		std::cout << "\nWhat is their class?\nStrongman [High Strength] - 0\nGunslinger [High Dexterity] - 1\nPlanner [High Intelligence] - 2\nCharmer [High Charisma] - 3\n";
		std::cin >> MemberClass;
		

		//Try catch for bad member class value
		try
		{
			Gang.AddMember(MemberName, MemberClass);
		}
		catch (Overflow &e)//prevents values from exceeding the allowed class range
		{
			std::cout << std::endl << e.what() << std::endl;
		}
		catch (Underflow &e)//prevents values from exceeding the allowed class range
		{
			std::cout << std::endl << e.what() << std::endl;
		}
		
		system("Pause");
		system("cls");
	}



	//This is the main loop where the user will order their gang to go on different heists
	while (Gang.getMoney() < (Gang.size() * PerMem) && Gang.size() > 0)
	{
		//This section shows the different options and the percent chance of the take they earn
		std::cout << "Commence Heist on Target (Select)." << std::endl;

		std::cout << "[1]-Rustle Cattle\n"; 
		examples[0]->DisplayChance(Gang.getStr(), Gang.getDex(), Gang.getInt(), Gang.getCh());

		std::cout << "\n[2]-Stage Coach\n";
		examples[1]->DisplayChance(Gang.getStr(), Gang.getDex(), Gang.getInt(), Gang.getCh());

		std::cout << "\n[3]-Bank Heist" << std::endl;
		examples[2]->DisplayChance(Gang.getStr(), Gang.getDex(), Gang.getInt(), Gang.getCh());

		std::cout << "\n[4]-Quit" << std::endl;


		//Select is used to hold the user's heist selection
		int select = 0;
		std::cin >> select;

		//This loop will prevent the user from putting in a value that doesn't correspond to a heist type
		while (select < 1 || select > 4 || !std::cin)
		{
			if (!std::cin)
			{
				std::cin.clear();
				std::cin.ignore(1000, '\n');
			}
			std::cout << "You have entered a bad value, please retry, but select a value between 1-3.\n";
			std::cin >> select;
		}

		//Uses the select value to push back the appropriate heist
		switch (select)
		{
		case 1:
			heists.push_back(new Rustle());
			break;
		case 2:
			heists.push_back(new Coach());
			break;
		case 3:
			heists.push_back(new Bank());
			break;
		case 4:
			return 1;
		}
		
		//this displays the "stages" of the heist
		heists[0]->printStage1();
		Sleep(2000);
		heists[0]->printStage2();
		Sleep(2000);
		heists[0]->printStage3();
		Sleep(2000);

		//This will get the results of the heist based on the gang's collective stats
		double result = heists[0]->CalculateOutcome(Gang.getStr(), Gang.getDex(), Gang.getInt(), Gang.getCh());
		currheist++;

		//This will activate if the gang has insufficient stats, the result will be a -1
		if (int(result) == -1)
		{
			
			try//Esnures that a member value that isn't there gets deleted
			{
				if (Gang.size() == 1)
				{
					Gang.arrest(0);
				}
				else
				{
					Gang.arrest(rand() % (Gang.size() - 1));
				}
			}
			catch (Underflow &e)
			{
				std::cout << e.what() << std::endl;
			}
		}
		else//If the user gets back a regular value then it will display this to the user
		{
			std::cout << Gang.getN() << " has earned money: \n"<< std::endl;
			std::cout << "Take: " << result << std::endl;
			//This will add money to the gang.
			Gang.addMoney(result);

			std::cout << "Needed to retire: " << ((Gang.size() * PerMem) - Gang.getMoney()) << "\n";
			
		}

		system("Pause");
		std::cout << "\n";

		//Calls the Run Job function of the gang object
		Gang.RunJob();

		//Checks if any member's can level up, if so it levels them up
		Gang.GetExperience();

		//This will be used to store a user response
		std::string confirm = "";

		system("cls");//Clears screen

		//This will check to see if the user can hire new gang recruits
		if (Gang.getMoney() < 400)//Displays a message if the user doesn't have enough money
		{
			std::cout << "Unfortunately, you don't have enough money to recruit new members.\n";
		}
		else if(Gang.getMoney() >= 400 && Gang.size() < 8)//displays a message if the user can hire a new recruit
		{
			std::cin.ignore(1000, '\n');
			std::cout << "Would you like to add any new members[Y/y - Yes]? This will cost $400" << std::endl;
			std::cout << "Adding members to your gang will make certain jobs much easier, but you will have to work longer to retire.\n";
			std::cout << "Any other value will be considered you declining this offer." << std::endl;
			getline(std::cin, confirm);

		}
		else//Displays a message if the user has too many members
		{
			std::cout << "Your gang is currently full, you can accept no more recruits\n";
		}

		//This will allow the user to hire new recruits
		while ((confirm == "Y" || confirm == "y") && Gang.size() < 8 && Gang.getMoney() >= 400.00)
		{
			//gets recruits name
			std::cout << "Enter your recruit's name...." << std::endl;
			getline(std::cin, MemberName);

			//gets recruit class value
			std::cout << "What class are they?\nStrong Man [High Strength] - 0\nGunslinger [High Dexterity] - 1\nPlanner [High Intelligence] - 2\nCharmer [High Charisma] - 3\n";
			std::cin >> MemberClass;

			//This will add the member as long as the user doesn't have too little money, and the gang isn't too big
			try
			{
				Gang.AddMember(MemberName, MemberClass);
				Gang.addMoney(-400.00);
			}
			catch(Overflow &e)
			{
				std::cout << "\nAddition Canceled\n";
				std::cout << e.what() << std::endl;
			}
			catch(Underflow &e)
			{
				std::cout << "\nAddition Canceled\n";
				std::cout << e.what() << std::endl;
			}

			std::cin.ignore(10000, '\n');

			//checks if user wants to add another member
			std::cout << "\n\nWould you like to add a member[Enter Y/y to continue]? Max: 8\n Current Size: " << Gang.size();
			std::cout << "\nAny other value will be considered you declining this offer." << std::endl;
			getline(std::cin, confirm);

			system("cls");

		}

		//flavor text
		std::cout << "\nYour gang relaxes at their hideout, the sun sets, and then a new day begins.\n" << std::endl;

		//Displays current gang condition
		std::cout << "Gang Size: " << Gang.size() << std::endl;
		std::cout << "Total Take: " << Gang.getMoney() << std::endl;
		std::cout << "Needed to retire: " << ((Gang.size() * PerMem) - Gang.getMoney()) << "\n";
		std::cout << "Total Heists: " << currheist << std::endl;

		//prevents too much memory from being taken up
		delete heists[0];
		heists.erase(heists.begin());

		system("pause");
		system("cls");
	}

	//Once the main loop is exited these two if statements will display an appropriate message for the user based on the success status of their mission
	if (Gang.getMoney() > (Gang.size() * PerMem))
	{
		std::cout << "You have enough money to retire.... your gang members will go there separate ways." << std::endl;
		std::cout << "Total Take: " << Gang.getMoney() << std::endl;
		std::cout << "Total Heists: " << currheist << std::endl;
		Gang.Disband();
	}
	else
	{
		std::cout << "Your gang fought hard to make themselves a future in a world that is everchanging, but just like many people, they couldn't make it.\n";
		std::cout << "Your gang has reached it's end, you made your mark on the world, but will you truly be remembered?\n";

		std::cout << "GAME OVER.....";
	}


	
	system("Pause");
}