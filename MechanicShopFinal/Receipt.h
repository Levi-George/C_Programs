/*
Program Name: Mechanic Shop
Author: Levi George
Date Last Updated: 12/20/2019
Program Purpose: To act as an interface for a mechanic's autobody/autorepair shop managing their parts, and customers, those that are waiting to be helped
and those who currently have their car in a bay being worked on. It will allow workers to modify the customer's receipt.

*/
#pragma once
#include <iostream>
#include <string>
#include <iomanip>

class Receipt
{
private:

	//receipt attributes
	double cost;
	std::string statedProblem;
	std::string changesMade = "Auto Body / Auto Repair Receipt: \n";
	
	//a $10 charge will be applied to the customer receipt per changes
	int numOfChanges = 0;

public:
	//constructors and destructor
	Receipt()
	{
		changesMade = changesMade + "Check - Up :\t" + std::to_string(cost);
	}
	Receipt(std::string problem)
	{
		changesMade = changesMade + "Check - Up :\t" + std::to_string(cost);
		statedProblem = problem;
	}
	~Receipt(){}

	//adds charge to receipt
	void addChange(double price, std::string change)
	{
		changesMade += "\n" + change + ": \t " + std::to_string(price);
		this->cost += price;
		numOfChanges++;
	}

	//display function outputs info
	void display()
	{
		std::cout << changesMade;
		std::cout << "Labour Cost: \t " + std::to_string(this->numOfChanges*10.00);
		std::cout << "Total Cost : \t " + std::to_string(this->cost + this->numOfChanges * 10.00) << "\n";
	}
	//returns output info
	std::string getChanges()
	{
		changesMade += "Labour Cost: \t " + std::to_string(this->numOfChanges*10.00);
		changesMade += "Total Cost : \t " + std::to_string(this->cost + this->numOfChanges * 10.00) + "\n";
		return changesMade;
	}


	//getter
	double getCost()
	{
		return (this->cost + this->numOfChanges * 10.00);
	}
	

};