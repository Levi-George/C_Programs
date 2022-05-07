#pragma once
/*
Program Name: I play too much red dead
Author: Levi George
Date Last Updaed: 5/11/2019
Program Purpose: To allow a user to play through several schemes to make money for their gang to retire
*/
#include <string>
class Target
{
public:
	//This is the constructor, it requires several attributes 
	Target(int str,int dex, int chr, int intellect, double take)
	{ 
		this->reqStr = str;
		Stats[0] = str;
		this->reqDex = dex;
		Stats[1] = dex;
		this->reqChrm = chr;
		Stats[2] = chr;
		this->reqInt = intellect;
		Stats[3] = intellect;
		this->take = take; 
	}
	//Destructor
	~Target(){}
	//calculates chance
	double calculateChance(int str, int dex, int smrt, int chr)
	{
		double Chance = 0;
		int check[4] = { str, dex, smrt, chr };

		for (int i = 0; i < 4; i++)
		{
			if (Stats[i] <= check[i])
			{
				Chance += 1;
			}
			else
			{
				Chance += double(check[i]) / double(Stats[i]);
			}
		}

		Chance = Chance / 4.0;

		//This is used to ensure presentability
		if (Chance > 1.0)
		{
			Chance = 1.00;
		}

		return Chance;
	}
	//Virtual function for the display of chance and calculation of the outcome
	virtual void DisplayChance(int str, int dex, int smrt, int chr) = 0;
	virtual double CalculateOutcome(int str, int dex, int smrt, int chr) = 0;

	//Returns the total take of the heist
	double getTake()
	{
		return take;
	}
	//Virtual functions for the print stages
	virtual void printStage1() = 0;
	virtual void printStage2() = 0;
	virtual void printStage3() = 0;
	//These return functions will return specific stat requirements
	int getStr()
	{
		return reqStr;
	}
	int getDex()
	{
		return reqDex;
	}
	int getChrm()
	{
		return reqChrm;
	}
	int getInt()
	{
		return reqInt;
	}
	
private:
	//These store values that are inherent to each heist 
	int reqStr, reqDex, reqChrm, reqInt;
	double take;
	int Stats[4] = { 0 };
	
};
