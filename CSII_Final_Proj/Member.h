#pragma once
/*
Program Name: I play too much red dead
Author: Levi George
Date Last Updaed: 5/11/2019
Program Purpose: To allow a user to play through several schemes to make money for their gang to retire
*/
//The member class will be used as the basis for the posse, each member will have specific attributes which aid in completing the mission without
//losing money
enum Class {Burly = 0, Quick = 1, Clever = 2, Charmer = 3};
#include "Horse.h"
#include <string>
#include <iostream>
#include <map>
#include <Windows.h>
#include "Except.h"

class member
{
private:

	int str, dex, smrt, chr, lvl = 0;//Specific stats
	int jobsran = 0;//jobs ran
	std::string name, job;//name and specific class
	Class C;//Class enum
	double money;

	//Enum to String Map
	std::map<Class, std::string> ClassMaps
	{
		{Burly, "Strongman"},
		{Quick, "Gunslinger"},
		{Clever, "Planner"},
		{Charmer, "Face"}
	};

public:
	//overloaded operator prototypes
	member operator++();
	friend std::ostream& operator<<(std::ostream&, const member&);

	//Parameterized constructor
	member(std::string n, int c)
	{
		this->name = n;//sets name

		//throws overflow exception
		if (c > 3 || c < 0)
		{
			std::string e = "Invalid Class, no class " + c;
			throw Overflow(e);
		}

		//sets enum and job string using a map
		this->C = Class(c);
		this->job = ClassMaps.at(C);

		//sets stats based on class
		switch (c)
		{
		case Burly:
			this->str = 5;
			this->dex = 2;
			this->smrt = 2;
			this->chr = 4;
			break;
		case Quick:
			this->str = 2;
			this->dex = 5;
			this->smrt = 4;
			this->chr = 3;
			break;
		case Clever:
			this->str = 2;
			this->dex = 3;
			this->smrt = 5;
			this->chr = 1;
			break;
		case Charmer:
			this->str = 2;
			this->dex = 2;
			this->smrt = 3;
			this->chr = 5;
			break;
		}

	}
	//destructor
	~member()
	{
	}
	//returns level stat
	int getLevel()
	{
		return this->lvl;
	}
	//increments jobs ran stat
	void RunJob()
	{
		jobsran++;
	}
	//returns total jobs ran
	int getJobsRan()
	{
		return jobsran;
	}
	
	//displays all values of the Member
	std::string toString() const
	{
		std::string print = "Name: " + name + "\nJob: " + job + "\nStats =====\n" + "Level: " + std::to_string(lvl) + "\nStrength: " + std::to_string(str);
		print += "\nDexterity: " + std::to_string(dex) + "\nIntelligence: " + std::to_string(smrt) + "\nCharisma: " + std::to_string(chr);

		
		return print;

	}
	//displays name
	std::string getName()
	{
		return name;
	}
	
	//Return the specific stat of each member
	int Charm()
	{
		return chr;
	}
	int Intelligence()
	{
		return smrt;
	}
	int Strength()
	{
		return str;
	}
	int Dexterity()
	{
		return dex;
	}
};
//overloaded pre-increment operator
member member::operator++()
{
	//Updates basic stats based on class
	this->lvl++;
	switch (this->C)
	{
	case Burly:
		this->str += 6;
		this->dex += 1;
		this->smrt += 1;
		this->chr += 2;
		break;
	case Quick:
		this->str += 1;
		this->dex += 7;
		this->smrt += 2;
		this->chr += 2;
		break;
	case Clever:
		this->str += 1;
		this->dex += 2;
		this->smrt += 6;
		this->chr += 1;
		break;
	case Charmer:
		this->str += 1;
		this->dex += 1;
		this->smrt += 3;
		this->chr += 7;
		break;
	}
	return *this;
}

//overloading stream output operator
std::ostream& operator<<(std::ostream& cout, const member& obj)
{
	cout << obj.toString();
	return cout;
}