#pragma once
/*
Program Name: I play too much red dead
Author: Levi George
Date Last Updaed: 5/11/2019
Program Purpose: To allow a user to play through several schemes to make money for their gang to retire
*/
#include "Target.h"
#include <string>
#include <iostream>

class Bank : public Target
{
private:
	
public:
	//Constructor
	Bank() : Target(50, 50, 50, 50, 4000)
	{

	}
	//destructor
	~Bank()
	{

	}
	//Prints flavor text
	void printStage1()
	{//writing the story lining
		std::cout << "\nYou and the gang staked out a bank in a far away town, you and your gang get ready to break down the front doors." << std::endl;
		std::cout << "You smash open the front doors and let out the classic phrase \"Every put your hands up! We only want the money!\"" << std::endl;
	}
	void printStage2()
	{
		std::cout << "\nYou quickly disarm the two guards next to the doors and starting forcing people to the ground" << std::endl;
		std::cout << "One of your men post's themself behind the door to keep watch for any unwanted guests, the rest keep an eye on the hostages." << std::endl;
		std::cout << "You head to the back of the bank near the vault and force the manager to open it, once inside you start loading cash into satchels.\n";
	}
	void printStage3()
	{
		std::cout << "\nWhen you get back to the front of the bank, your men have tied everyone up, but some law men have already arrived." << std::endl;
		std::cout << "\nYou and your men head toward the back, get on your horses and flee." << std::endl;
	}
	//Returns a message for the user to view
	void DisplayChance(int str, int dex, int smrt, int chr)
	{
		//Calculates chance
		double chance = calculateChance(str, dex, smrt, chr);
		
		//displays chance
		std::cout << std::fixed << std::setprecision(2);
		std::cout << "Success: " << 100.0 * chance << "% " << std::endl;
	}
	//Calculates results, displays messages based on results and returns the result
	double CalculateOutcome(int str, int dex, int smrt, int chr)
	{
		//Calculates chance
		double chance = calculateChance(str, dex, smrt, chr);

		//This set of if-else if statements will display a message and return the result based on the chance value
		if (chance < .25)
		{
			std::cout << "\nThe lawmen quickly took notice of you fleeing through the back and give chase,\nIn the ensuing firefight one of your men's horse was shot and you had to" << std::endl;
			std::cout << "leave a man behind...." << std::endl;
			return -1;
		}
		else if (chance >= .25 && chance < .50)
		{
			std::cout << "\nIn your rush to leave, you drop one of the bags, the coinage in the bag made a noticeable sound and law men spotted you as you made your escape." << std::endl;
			std::cout << "One of the law men opened fire, the round missed any of your men but hit one of the bags, the heat from the round lit the bag on fire." << std::endl;
			return getTake() * .25;
		}
		else if (chance >= .50 && chance < .75)
		{
			std::cout << "\nYou rushed while filling bags and forgot to fill one of the bags that you brought along." << std::endl;
			return getTake() * .5;
		}
		else if (chance >= .75 && chance < 1)
		{
			std::cout << "\nThe manager locked himself inside the vault meaning you could only collect money from the cashiers and safety deposit boxes not locked behind the massive door." << std::endl;
			std::cout << "Luckily for you and your men, there was spring cleaning going on, so plenty of money was left lying around.\n";
			std::cout << "\n" << std::endl;
			return getTake() * .75;
		}
		else
		{
			std::cout << "You were able to grab all the money your bags could hold and even found a gold bar among the heaps of cash.\n" << std::endl;
			return getTake() * 1;
		}
	}

};