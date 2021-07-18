/*
Program Name: Mechanic Shop
Author: Levi George
Date Last Updated: 12/20/2019
Program Purpose: To act as an interface for a mechanic's autobody/autorepair shop managing their parts, and customers, those that are waiting to be helped
and those who currently have their car in a bay being worked on. It will allow workers to modify the customer's receipt.

*/

#pragma once

#include <string>
#include <iostream>

class Part
{
private:
	//part attributes
	std::string partName;
	int inventory, partCode;
	double cost;
public:
	//constructors and destructor
	Part(){}
	~Part(){}
	Part(std::string name, int code, double cost, int inventory)
	{
		this->partName = name;
		this->partCode = code;
		this->cost = cost;
		this->inventory = inventory;
	}

	//setters
	void setInventory(int inventory)
	{
		this->inventory += inventory;
	}

	//getters
	double getCost()
	{
		return this->cost;
	}
	std::string getName()
	{
		return this->partName;
	}

	int getCode()
	{
		return this->partCode;
	}

	int getInven()
	{
		return this->inventory;
	}

	//display function
	void print()
	{
		std::cout << "\nPart Information: \n";
		std::cout << "\nPart Name: " << partName;
		std::cout << "\nPart Code: " << partCode;
		std::cout << "\nPart Inventory: " << inventory;
		std::cout << "\nPart Cost: " << cost << "\n";
	}

	//operator overloads for ease of insertion into binary tree
	bool operator==(Part &Obj)
	{
		if (this->partCode == Obj.getCode())
			return true;
		else
			return false;
	}

	bool operator<(Part &Obj)
	{
		if (this->partCode < Obj.getCode())
			return true;
		else
			return false;
	}
	bool operator>(Part &Obj)
	{
		if (this->partCode > Obj.getCode())
			return true;
		else
			return false;
	}
};

