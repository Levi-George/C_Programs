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

class Coach : public Target
{
private:
	
public:
	//Constructor, the values passed determine the difficulty and the total possible money to be earned from the heist
	Coach() : Target(30, 30, 30, 30, 600.0)
	{

	}
	//destructor
	~Coach()
	{

	}
	//These printStage methods display flavor text
	void printStage1()
	{//writing the story
		std::cout << "\nYou and the gang cut off a stage coach, the shotgun rider pulls out his rifle and aims it, but you shoot it out of his hand" << std::endl;
		std::cout << "The gang quickly disarms the coach driver, and passengers of any weapons and money." << std::endl;
	}
	void printStage2()
	{
		std::cout << "\nYou mount the coach and your gang hogtie the stage coach entourage, you head off while your gang finishes up" << std::endl;
		std::cout << "You hope that your gang has an easy time getting back as you drive the coach to a fence." << std::endl;
	}
	void printStage3()
	{
		std::cout << "\nYou bring the stage coach up to the fence's hideout and park it behind some foliage" << std::endl;
		std::cout << "the fence owner confronts you with a stack of \"fat cash\"." << std::endl;
	}
	//This will display the chance of success
	void DisplayChance(int str, int dex, int smrt, int chr)
	{
		double chance = calculateChance(str, dex, smrt, chr);

	
		std::cout << std::fixed << std::setprecision(2);
		std::cout << "Success: " << 100.0 * chance << "% " << std::endl;
	}
	//This will return the value of success times the possible take for the proper take earned
	//It will also display a message based on the user's success
	double CalculateOutcome(int str, int dex, int smrt, int chr)
	{
		//calculates chance
		double chance = calculateChance(str, dex, smrt, chr);

		//returns values and displays messages
		if (chance < .25)
		{
			std::cout << "\nOne of the hogtied put up a fight, the ensuing raccous results in some local law taking notice, your gang ended up getting roughed up and you lost a member." << std::endl;
			return -1;
		}
		else if (chance >= .25 && chance < .50)
		{
			std::cout << "\nAll of the hogtied get free and had some guns hidden on their persons. They open fire on the coach and your gang, no one was hurt but you lost some of the loot and the coach was banged up." << std::endl;
			return getTake() * chance;
		}
		else if (chance >= .50 && chance < .75)
		{
			std::cout << "\nYou ran the stage coach through some rough terrain, your less than stellar handling results in a good deal of damage to the coach" << std::endl;
			std::cout << "when you bring it in the fence gives you a fraction of what you could've earned" << std::endl;
			return getTake() * chance;
		}
		else if (chance >= .75 && chance < 1)
		{
			std::cout << "\nYou ran the stage coach back to the fence successfully, but one of your gang members got into a fight with a boar that wanted a bag of the loot \n" << std::endl;
			std::cout << "\n" << std::endl;
			return getTake() * chance;
		}
		else
		{
			std::cout << "\nYou ran the stage coach back to the fence successfully and your gang made it back to camp with the loot.\n" << std::endl;
			return getTake() * chance;
		}
	}

};