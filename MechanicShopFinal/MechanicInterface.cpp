/*
Program Name: Mechanic Shop
Author: Levi George
Date Last Updated: 12/20/2019
Program Purpose: To act as an interface for a mechanic's autobody/autorepair shop managing their parts, and customers, those that are waiting to be helped
and those who currently have their car in a bay being worked on. It will allow workers to modify the customer's receipt.

related files
customers.txt
members.txt
partsCosts.txt
errorCodes.txt

*/

//utility classes
#include <iostream>
#include <string>
#include <fstream>


//Data Structures
#include "BSTree.h"
#include "HashTable.h"
#include <map>
#include <set>//don't think I used this
#include "Queue.h"
#include <vector>//don't think I used this

//Base Classes
#include "Customer.h"
#include "Car.h"
#include "Receipt.h"
#include "Parts.h"

///This counts as one of the four data structures, Map
std::map<int, std::string> MAKE = {
	{ 0, "Ford" },
	{ 1, "Chevy" },
	{ 2, "Honda" },
	{ 3, "Subaru" },
	{ 4, "Mercedes" },
	{ 5, "Toyota" }

};

//This template function will work as a validator, accepting two values as parameters
//It should return a valid value between the two values as long as two are provided.
template<class T>
T validator(T ll, T ul, T val)
{
	//Runs while val is less than the lower limit (ll) and greater than the upper limit (ul)
	while (!std::cin || val > ul || val < ll)
	{
		//when the stream breaks this will clear the stream and ignore any characters 
		if (std::cin.fail())
		{
			std::cout << "You need to enter proper input, no symbols or letters\n";
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}//prompts for input
		else if (val > ul || val < ll)
		{
			std::cout << "You need to enter a value between " << ll << " and " << ul << "\n";
		}

		//accepts input
		std::cin >> val;
	}

	
	return val;
}

void addCustomer(Queue &CustomerReady)
{
	std::string name, number, make, model, vin;//holds user information
	std::map<int, std::string>::iterator makeIt;//iterator for make of a car map
	int choice;
	int year = 0;

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//clears input buffer
	std::cout << "\nYou will begin entering the customer information\n";

	//gets input from user
	std::cout << "What is the customer's name?\n";
	std::getline(std::cin, name);

	std::cout << "\nWhat is the customer's phone number?\n";
	std::getline(std::cin, number);

	std::cout << "\nWhat is the make of their car?\n";

	int i = 0;

	//outputs each car maker
	for (unsigned int j = 0; j < MAKE.size(); j++)
	{
		std::cout << j << ". " << MAKE.at(j) << "\n";
		i = signed int(j);
	}

	std::cin >> choice;

	//validates input
	choice = validator(0, i, choice);
	make = MAKE.at(choice);

	//clears buffer
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	//gets input
	std::cout << "\nWhat is the model of their car?\n";
	std::getline(std::cin, model);

	std::cout << "\nWhat is the year of the car?\n";
	std::cin >> year;

	//validates year
	year = validator(1900, 2019, year);

	//clear buffer
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	//gets input
	std::cout << "\nWhat is the vin of their car?\n";
	getline(std::cin, vin);

	//populates new customer and car
	Customer * newCust = new Customer(name, number);
	Car * newCar = new Car(make, model, vin, year);

	newCust->setCar(newCar);//sets customers car
	CustomerReady.Enqueue(newCust);//places customer into queue
}

//will add a charge to a customer
void addCharge(Queue &CustomerReady, BSTree &Storage)
{
	//holds user input
	std::string changes;
	int code;

	//clears input buffer
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	//gets input
	std::cout << "What change did you make?\n";
	getline(std::cin, changes);

	std::cout << "What part did you use? (Enter code)";
	std::cin >> code;

	//gets part from binary tree
	Part * gotPart = Storage.getNode(Storage.rootNode(), code);

	//gets customer from queue
	Customer * existingCust = CustomerReady.Peek();

	//gets cost of part
	double cost = gotPart->getCost();

	//adds charge to the customer
	existingCust->addCharge(cost, changes);
}

//closes the customers receipt, uses the queue and hash table
void closeCharge(Queue &CustomerReady, HashTable &members)
{
	//holds user choice
	int choice = 0;
	//prompts for information
	std::cout << "Does the customer want to join the club?\n[1] Yes\n[2] No\n";
	std::cin >> choice;
	//gets customer from the top of the queue
	Customer * existingCust = CustomerReady.Peek();

	//validate the user choice
	do
	{
		if (!std::cin || choice < 1 || choice > 2)
		{
			std::cout << "Bad input, retry";
			std::cin >> choice;
		}
	} while (!std::cin || choice < 1 || choice > 2);

	//gets user input on the new id for the member
	if (choice == 1)
	{
		//prompts for input
		std::cout << "What is the customer's ID?\n";
		std::cin >> choice;

		//validates user input
		do
		{
			if (!std::cin || choice < 0)
			{
				if (!std::cin)
				{

				}
				std::cout << "Enter a value greater than 0\n";
				std::cin >> choice;
			}
		} while (!std::cin || choice < 0);

		//sets the customer id
		existingCust->setID(choice);

		//inserts the customer into the hash table of members
		members.insert(*existingCust);

	}

	//removes the customer that was inserted into the hash table
	std::cout << "Closing Customer\n";
	CustomerReady.dequeue();
}

//edits a customer, closes a receipt or adds a change
void editCustomer(Queue &customerReady, HashTable &members, BSTree &Storage)
{
	//clears input buffer
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


	//checks if the queue is empty
	if (customerReady.Peek() != nullptr)
	{
		std::cout << "\nYou will be editing this customer today and car today.\n";
		customerReady.Peek()->toString();
	}
	else
	{
		std::cout << "\nCurrently no customers waiting to be serviced...\n";
		return;
	}

	//prompts the user for information
	std::cout << "\nWhat changes would you like to make today?\n";
	std::cout << "[1] Add charge to a receipt\n[2] Close receipt\n";

	//holds user input
	int choice = 0;

	//retrieves user input
	std::cin >> choice;
	
	//runs depending on the use selection
	switch (choice)
	{
		case 1:
			addCharge(customerReady, Storage);
			break;
		case 2:
			closeCharge(customerReady, members);
			break;
	}

}

//gets data from files in the program source
void uploadData(Queue&, std::multimap<std::string, std::string>&, HashTable&, BSTree&);
//outputs data to the files from the program source
void downloadData(Queue&, std::multimap<std::string, std::string>&, HashTable&, BSTree&);
//changes the inventory of a parts in the binary tree
void orderParts(BSTree&);

int main()
{
	///This counts as three of the four data structures, Queue, Binary Tree, and a Set
	Queue customerReady;//for customers	
	std::multimap<std::string, std::string> errorCodes;//for error code lookup, this might potentially be an extra portion of the program to include ... needs to be stored
	HashTable members;//holds the members of the shop ... needs to be stored
	BSTree Storage;//for parts 

	//gets data from source files
	uploadData(customerReady, errorCodes, members, Storage);

	//holds user input
	int response = 0;
	bool run = true;

	//runs while user response is not -1
	while (run)
	{
		//prompts user for input
		std::cout << "Welcome to the auto body/repair shop management system,\n";
		std::cout << "What would you like to do? Enter negative one to quit\n[1] Add Customer\n[2] Modify Customer\n";
		std::cout << "[3] Check Inventory\n[4] Order Parts\n[5] View Members\n[6] View Error Codes\n[7] Add Error Codes\n";

		//validates input
		do
		{
			if (response == -1)
			{
				run = false;
			}
			if (!std::cin)
				std::cout << "Bad input, retry\n";
			if (response < 1 || response > 7 && response != -1)
				std::cout << "Enter a value between 1 and 7\n";
			std::cin >> response;
		} while (!std::cin || response < 1 || response > 7 && response != -1);
		
		//decision structure
		if (response == 1)//Add customer to queue
		{
			addCustomer(customerReady);
		}
		else if (response == 2)//editing customer queue
		{
			editCustomer(customerReady, members, Storage);
		}
		else if (response == 3)//part lookup
		{
			//gets input
			int code;
			std::cout << "Enter a part code\n";
			std::cin >> code;

			//prevents looking up parts when there are none
			if (Storage.rootNode() == nullptr)
			{
				std::cout << "Inventory is empty\n";
			}
			else
			{
				Storage.findNode(Storage.rootNode(), code);
			}

			std::cout << "\n";

		}
		else if (response == 4)//part inventory
		{
			orderParts(Storage);
		}
		else if (response == 5)//View/Edit members
		{
			int code;
			//gets input
			

			//prevents looking up members when they don't exist
			if (members.isEmpty())
			{
				std::cout << "\nThere are no members currently\n";
			}
			else
			{
				//input retrieval
				std::cout << "What customer do you wish to view? (Enter their code)\n";
				std::cin >> code;

				//input validation
				while (!std::cin)
				{
					std::cin.ignore(1000, '\n');
					std::cin.clear();

				}

				//displays info
				members.getNode(code)->toString();
			}
		}
		else if (response == 6)//View Error Codes
		{
			std::cout << "Error Code :: Explanation\n";
			
			//iterates through map to display each error code
			std::multimap<std::string, std::string>::iterator it;
			for (it = errorCodes.begin(); it != errorCodes.end(); it++)
			{
				std::cout << it->first << " :: " << it->second << std::endl;
			}
		}
		else if (response == 7)//Add error code
		{
			//holds user info
			std::string code, descript;
			std::pair<std::string, std::string> newpair;

			//clears buffer
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			//input retrieval
			std::cout << "Enter the code\n";
			getline(std::cin, code);

			std::cout << "Enter the description\n";
			getline(std::cin, descript);			

			//loads newpair with user info
			newpair.first = code;
			newpair.second = descript;

			//inserts newpair into error code map
			errorCodes.insert(newpair);
		}
	}

	//downloads all data into files
	downloadData(customerReady, errorCodes, members, Storage);
}

//order part function
//changes inventory of parts in tree
void orderParts(BSTree & Storage)
{
	int choice = 0;

	//if no items are in the tree no inventory can be changed
	if (Storage.rootNode() == nullptr)
	{
		std::cout << "No parts exist, you can't order any, canceling operation\n";
		return;
	}
	//stores user input, prompts for input
	int code;
	int quan = 0;
	std::cout << "Enter a part code\n";
	std::cin >> code;

	//validates input
	while (!std::cin)
	{
		std::cin.clear();
		std::cin.ignore(1000, '\n');
		std::cout << "Bad input, retry\n";
		std::cin >> code;
	}

	//retrieves inventory and information of part
	Storage.findNode(Storage.rootNode(), code);

	//prompts for input
	std::cout << "Add parts?\n[1] Yes\n[2] No\n";
	std::cin >> choice;

	//validates input
	choice = validator(1, 2, choice);

	//retrieves and validates input
	std::cout << "How many?";
	std::cin >> quan;
	quan = validator(1, 100, quan);

	//declares new part
	Part * edit;

	//populates part with desired part
	edit = Storage.getNode(Storage.rootNode(), code);

	//applies inventory update to part if it exists
	if (edit == nullptr)
	{
		std::cout << "Inventory item not found, restock canceled\n";
	}
	else
	{
		edit->setInventory(quan);
	}
}

//takes data from files and imports it into it's respective structure
void uploadData(Queue& customerReady, std::multimap<std::string, std::string>& errorCodes, HashTable & members, BSTree & Storage)
{
	//declares streams
	std::fstream partsListGet;
	std::fstream errorCodeGet;
	std::fstream membersGet;
	std::fstream customerGet;

	//opens files
	partsListGet.open("partsCost.txt", std::ios::in);
	errorCodeGet.open("errorCodes.txt", std::ios::in);
	membersGet.open("members.txt", std::ios::in);
	customerGet.open("customers.txt", std::ios::in);

	//populates binary tree of parts
	while (!partsListGet.eof())
	{
		//holds part info
		std::string name;
		int ID, inventory = 0;
		double cost;

		//obtains part info from file
		partsListGet >> name >> ID >> cost >> inventory;

		//instantiates new part and inserts into tree
		Part * newpart = new Part(name, ID, cost, inventory);
		Storage.insertNode(newpart, Storage.rootNode());
	}
	//populates multimap of error codes
	//runs while file is not empty
	while (!errorCodeGet.eof())
	{
		//holds code info
		std::string code, descript;

		//grabs code info
		getline(errorCodeGet, code);
		getline(errorCodeGet, descript);

		//instantiates pair
		std::pair<std::string, std::string> pair;

		//populates pair
		pair.first = code;
		pair.second = descript;

		//inserts pair into map
		errorCodes.insert(pair);
	}

	//declares name to hold info, runs while input is being grabbed
	std::string name;
	while (membersGet >> name)
	{
		//holds member info
		std::string number;
		std::string make, model, vin;
		int year;
		Car	* custCar;
		int ID;

		//gets member info
		membersGet >> number >> ID >> make >> model >> vin >> year;

		//instantiates and populates customer and their car
		custCar = new Car(make, model, vin, year);
		Customer * newCust = new Customer(name, number, ID);
		newCust->setCar(custCar);

		//inserts member into hash table
		members.insert(*newCust);
	}

	//imports customer info into queue
	while (customerGet >> name)
	{
		//holds info of the customer
		std::string number;
		std::string make, model, vin;
		int year;
		Car	* custCar;
		int ID;
		double cost;
		std::string changes;

		//gets basic customer info
		membersGet >> number >> ID >> make >> model >> vin >> year >> cost;
		std::getline(membersGet, changes);
		
		//populates customer and customer car
		custCar = new Car(make, model, vin, year);
		Customer * newCust = new Customer(name, number, ID);
		newCust->setCar(custCar);

		//inserts customer into queue
		customerReady.Enqueue(newCust);
	}

	//closes files
	partsListGet.close();
	errorCodeGet.close();
	membersGet.close();
	customerGet.close(); 
}

//exports info from program's structures into data files 
void downloadData(Queue& customerReady, std::multimap<std::string, std::string>& errorCodes, HashTable & members, BSTree & Storage)
{
	//declares file streams
	std::fstream partsListGet;
	std::fstream errorCodeGet;
	std::fstream membersGet;
	std::fstream customerGet;

	//opens files
	partsListGet.open("partsCost.txt", std::ios::out | std::ios::trunc);
	errorCodeGet.open("errorCodes.txt", std::ios::out | std::ios::trunc);
	membersGet.open("members.txt", std::ios::out | std::ios::trunc);
	customerGet.open("customers.txt", std::ios::out | std::ios::trunc);
	
	//holds the middle code(ID number of the part) of the binary tree
	//this is done as a poor attempt to offer some semblance of balance to the binary tree
	int greatest = Storage.getGreatest();
	Part * storeMe = Storage.getNode(Storage.rootNode(), greatest / 2);

	//outputs part info to file
	partsListGet << storeMe->getName() << " " << storeMe->getCode() << " " << storeMe->getCost() << " " << storeMe->getInven() << "\n";

	//prepares to output other parts to file
	while (!Storage.isEmpty())
	{
		//runs for every item in the tree
		for (int i = 1; i < greatest+1; i++)
		{
			//Since we already did this for the desired root node.
			if (i == greatest / 2)
			{
				i++;
			}
			//gets part
			storeMe = Storage.getNode(Storage.rootNode(), i);

			//stores part info to file 
			partsListGet << storeMe->getName() << " " << storeMe->getCode() << " " << storeMe->getCost() << " " << storeMe->getInven() << "\n";
		}

		//deletes tree
		Storage.deleteTree(Storage.rootNode());
	}
	//gets all items from hash table
	for (int i = 0; i < members.getBucketSize(); i++)
	{
		//runs while a specific section of the hash table isn't empty
		while (!members.isBucketIndexEmpty(i))
		{
			//gets customer on end of hash table
			Customer storeMe = members.deleteEnd(i);

			//outputs member info into file
			membersGet << storeMe.getName() << " " << storeMe.getNumber() << " " << storeMe.getID() << " ";
			membersGet << storeMe.getCarMake() << " " << storeMe.getCarModel() << " " << storeMe.getCarVIN() << " " << storeMe.getCarYear() << "\n";
			//std::cout << "\n" << i << "\n"; ---- this was used for debugging purposes
		}
	}
	//runs while queue for customers is not empty
	while (!customerReady.isEmpty())
	{
		//gets customer from queue
		Customer storeMe = customerReady.dequeue();

		//stores customer info into file
		customerGet << storeMe.getName() << " " << storeMe.getNumber() << " " << storeMe.getID() << " ";
		customerGet << storeMe.getCarMake() << " " << storeMe.getCarModel() << " " << storeMe.getCarVIN() << " " << storeMe.getCarYear() << " " << storeMe.getCost() << "\n";
		customerGet << storeMe.getChanges() << " \n";
		
	}
	//iterator, iterates through multimap to export error code info
	std::multimap<std::string, std::string>::iterator it;
	for (it = errorCodes.begin(); it != errorCodes.end(); it++)
	{
		errorCodeGet << it->first << "\n" << it->second << "\n";
		
	}

	//closes files
	partsListGet.close();
	errorCodeGet.close();
	membersGet.close();
	customerGet.close();
}
