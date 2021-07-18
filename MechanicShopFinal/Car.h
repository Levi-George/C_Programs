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


class Car
{
private:
	//car attributes
	std::string make, model, vin;
	int year;

public:
	//constructors and destructor
	Car(){}
	Car(std::string make, std::string model, std::string vin, int year)
	{
		this->make = make;
		this->model = model;
		this->vin = vin;
		this->year = year;
		//add basic view charge
	}
	~Car(){}

	//getters
	int getYear()
	{
		return year;
	}

	std::string getVIN()
	{
		return this->vin;
	}
	std::string getMake()
	{
		return this->make;
	}
	std::string getModel()
	{
		return this->model;
	}

	//display functions
	std::string toString()
	{
		std::string information;
		information = std::to_string(year) + " " + make + " " + model;
		information += "\nVIN: " + vin;
		return information;
	}
	void display()
	{
		std::cout << std::to_string(year) << " " << make << " " << model;
		std::cout << "\nVIN: " << vin;
	}
};