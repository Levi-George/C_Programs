#pragma once
/*
Program Name: I play too much red dead
Author: Levi George
Date Last Updaed: 5/11/2019
Program Purpose: To allow a user to play through several schemes to make money for their gang to retire
*/
#include <string>

//This is a exception handling class, it will be used to display an error message and handle exceptions where a lower limit has been exceed
class Underflow
{
private:
	std::string message = "Underflow error";
public:
	//constructor
	Underflow()
	{

	}
	//parameterized constructor
	Underflow(std::string EM)
	{
		message = EM;
	}
	//destructor
	~Underflow()
	{

	}
	//accesses string 
	std::string what()
	{
		return message;
	}

};

//This will be used to catch exceptions that exceed an upper limit
class Overflow
{
private:
	std::string message = "Underflow error";//error message
public:
	//constructor
	Overflow()
	{

	}
	//parameterized constructor
	Overflow(std::string EM)
	{
		message = EM;
	}
	//destructor
	~Overflow()
	{

	}
	//Displays error message
	std::string what()
	{
		return message;
	}

};