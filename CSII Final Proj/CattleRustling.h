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
#include <iomanip>

class Rustle : public Target
{
private:
	double actualtake;
public:
	//Constructor, the values passed determine the difficulty and the total possible money to be earned from the heist
	Rustle() : Target(20, 20, 20, 20, 250.0)
	{

	}
	//deconstructor
	~Rustle()
	{

	}
	//These printStage methods display flavor text
	void printStage1()
	{
		std::cout << "\nYou and the gang come up on the herd of cattle,\na couple of ranchers come up to you and brusquely tell you to watch yourselves" << std::endl;
		std::cout << "The gang quickly pull their guns and disarm the ranchers." << std::endl;
	}
	void printStage2()
	{
		std::cout << "\nYou mount your horses and start guiding the cattle, you are taking them to the next town to sell them, although you" << std::endl;
		std::cout << "feel that you might lose some of the cattle along the way." << std::endl;
	}
	void printStage3()
	{
		std::cout << "\nYou see the town out in the distance, upon arriving at the local auctioneering yard, you and a few of your members split up" << std::endl;
		std::cout << "to sell the cattle to different buyers in order to avoid attracting too much attention." << std::endl;
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
		double chance = calculateChance(str, dex, smrt, chr);
		
		//returns values and displays messages
		if (chance < .25)
		{
			std::cout << "One of the vendors you sold some of the cattle to, ratted you to the sheriff, your gang tried to make a break for it, but you lost a man.\n" << std::endl;
			return -1;
		}
		else if (chance >= .25 && chance < .50)
		{
			std::cout << "\nNo one was paying attention and you lost a good chunk of the cattle, when you tried to get them back they ran off into a forest.\n" << std::endl;
			return getTake() * .25;
		}
		else if (chance >= .50 && chance < .75)
		{
			std::cout << "\nYou ran the cattle through some rough terrain, they ended up getting beat up, when you got them to the vendors they would" << std::endl;
			std::cout << "only take them for a lower price.\n" << std::endl;
			return getTake() * .50;
		}
		else if(chance >= .75 && chance < 1)
		{
			std::cout << "\nThe gang only lost one or two cattle and took them along a safe route, when you got to town you sold them for their best price\n" << std::endl;
			std::cout << "\n" << std::endl;
			return getTake() * .75;
		}
		else
		{
			std::cout << "\nThe gang didn't lose any cattle, and got them to town quickly, once in town you all were able to negotiate a excellent price for your cattle\n" << std::endl;
			return getTake() * 1;
		}
	}

};