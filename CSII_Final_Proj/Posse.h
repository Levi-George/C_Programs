#pragma once
/*
Program Name: I play too much red dead
Author: Levi George
Date Last Updaed: 5/11/2019
Program Purpose: To allow a user to play through several schemes to make money for their gang to retire
*/
#include <vector>
#include <string>
#include "Member.h"
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include "Except.h"

//This is essentially an exaggerated vector
class Posse
{
public:
	//Constructor 
	Posse(std::string n)
	{
		this->name = n;
	}
	//destructor
	~Posse()
	{
		
	}
	
	void GetExperience()
	{
		for (unsigned int i = 0; i < Gang.size(); i++)
		{
			if (Gang[i].getJobsRan() == 3)
			{
				std::cout << "A member has leveled up; New Stats: " << std::endl;
				++Gang[i];
				std::cout << Gang[i].toString() << std::endl;
				std::cout << std::setw(35) << std::setfill('=') << "\n";
				Sleep(5000);
			}
			if (Gang[i].getJobsRan() == 7)
			{
				std::cout << "A member has leveled up; New Stats: " << std::endl;
				++Gang[i];
				std::cout << Gang[i].toString() << std::endl;
				std::cout << std::setw(35) << std::setfill('=') << "\n";
				Sleep(5000);
			}
			
		}
	}
	std::string getN()
	{
		return name;
	}
	void Display()
	{
		for (unsigned int i = 0; i < Gang.size(); i++)
		{
			std::cout << '\n' << Gang[i].toString() << '\n';
		}
	}
	int size()
	{
		return Gang.size();
	}
	double getMoney()
	{
		return totaltake;
	}

	//These functions will be used calculate the total charisma, strength, dexterity, and intelligence of the gang
	int getCh()
	{
		int ch = 0;
		for (unsigned int i = 0; i < Gang.size(); i++)
		{
			ch += Gang[i].Charm();
		}
		return ch;
	}
	int getStr()
	{
		int str = 0;
		for (unsigned int i = 0; i < Gang.size(); i++)
		{
			str += Gang[i].Strength();
		}
		return str;
	}
	int getDex()
	{
		int dex = 0;
		for (unsigned int i = 0; i < Gang.size(); i++)
		{
			dex += Gang[i].Dexterity();
		}
		return dex;
	}
	int getInt()
	{
		int smart = 0;
		for (unsigned int i = 0; i < Gang.size(); i++)
		{
			smart += Gang[i].Intelligence();
		}
		return smart;
	}
	

	//setters
	//This will calculate the new total take, it prevents money from being totally depleted.
	void addMoney(double dinero)
	{
		if ((totaltake + dinero) < 0)
		{
			throw Underflow("Not enough money");
		}
		this->totaltake += dinero;
	}
	//This function creates a new member and prevents bad values from being used
	void AddMember(std::string name, int clas)
	{
		//Gang member overflow
		if (Gang.size() > 8)
		{
			throw Overflow("Gang filled");
		}
		if (clas > 3)//Class overflow
		{
			throw Overflow("No Classes in this range, Class Range: [0-3]");
		}
		if (clas < 0)//Class underflow
		{
			throw Underflow("No Classes in this range, Class Range: [0-3]");
		}

		//Prevents bad class values
		try
		{
			Gang.push_back(member(name, clas));
		}
		catch (Overflow &e)
		{
			std::cout << e.what() << std::endl;
		}
		
		
	}
	//This will delete every object in the vector as part of the end of the program
	void Disband()
	{
		if (totaltake > (Gang.size() * 750) || Gang.size() < 1)
		{
			for (unsigned int i = 0; i < Gang.size(); i++)
			{
				Gang.erase(Gang.begin() + i);
			}
		}
	
	}
	//This will pull out a member from the vector, and throw an exception if the size of the vector is 0 
	void arrest(int i)
	{
		if (Gang.size() == 0)
		{
			throw(Underflow("No Member's Left!"));
		}
		std::cout << Gang[i].getName() << " has been arrested\n" << std::endl; // , their horse " << Gang[i]->getHname() << " has been confiscated." << std::endl;
		Gang.erase(Gang.begin()+i);
	}
	//This will give every current member a increase in their jobsran stat which will indicate if they are ready to level up
	void RunJob()
	{
		std::cout << "The gang has gained experience." << std::endl;
		for (unsigned int i = 0; i < Gang.size(); i++)
		{
			Gang[i].RunJob();
		}
	}
	
private:
	std::vector<member> Gang;//gang vector, holds members
	double totaltake;//Total money that the gang has
	std::string name;//Name of the gang
};