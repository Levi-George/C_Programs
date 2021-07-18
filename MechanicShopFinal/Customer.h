#pragma once
/*
Program Name: Mechanic Shop
Author: Levi George
Date Last Updated: 12/20/2019
Program Purpose: To act as an interface for a mechanic's autobody/autorepair shop managing their parts, and customers, those that are waiting to be helped
and those who currently have their car in a bay being worked on. It will allow workers to modify the customer's receipt.

*/

#include <iostream>
#include <string>
#include "Car.h"
#include "Receipt.h"

class Customer
{
private:
	//customer attributes
	std::string name, number;
	Car	custCar;
	Receipt custRcpt;
	int ID;

public:

	//constructors, destructor
	Customer()
	{}

	Customer(std::string name, std::string number)
	{
		this->name = name;
		this->number = number;
		
	}
	Customer(std::string name, std::string number, int ID)
	{
		this->name = name;
		this->number = number;
		this->ID = ID;
	}
	~Customer()
	{

	}

	//setter
	void setCar(Car * newCar)
	{
		this->custCar = *newCar;
	}
	void setID(int ID)
	{
		this->ID = ID;
	}

	//getters
	std::string getName()
	{
		return name;
	}
	std::string getNumber()
	{
		return number;
	}
	int getID()
	{
		return this->ID;
	}
	std::string getCarMake()
	{
		return custCar.getMake();
	}
	std::string getCarModel()
	{
		return custCar.getModel();
	}
	std::string getCarVIN()
	{
		return custCar.getVIN();
	}

	int getCarYear()
	{
		return custCar.getYear();
	}

	std::string getChanges()
	{
		return custRcpt.getChanges();
	}

	double getCost()
	{
		return custRcpt.getCost();
	}

	void getReceipt()
	{
		custRcpt.display();
	}


	//display functions
	void toString()
	{
		std::cout << "Customer name: " << this->name << std::endl;
		std::cout << "Customer number: " << this->number << std::endl;
		this->getCarInfo();
	}
	void getCarInfo()
	{
		std::cout << custCar.toString();
	}

	//add charge function includes new charge and description on customer receipt
	void addCharge(double cost, std::string change)
	{
		if (cost == NULL)
		{
			std::cout << "\nCode does not exist\n";
		}
		custRcpt.addChange(cost, change);
	}

	//kills receipt when the customer is done at the shop
	double payAccount()
	{
		double amountPayed = custRcpt.getCost();
		custRcpt.~Receipt();
		return amountPayed;
	}




};