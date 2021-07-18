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
#include "Customer.h"


//hash function is ID%bucket size --- simple and easy

//template<class Type>
class HashTable
{
private:
	//node, bucket, and bucket size definition
	struct Node
	{
		Customer payload;
		Node * Point;
	};
	Node* bucket[12];
	const int BUCKET_SIZE = 12;

public:
	//constructor and destructor
	HashTable()
	{
		for (int i = 0; i < BUCKET_SIZE; i++)
		{
			bucket[i] = nullptr;
		}
	}
	~HashTable(){}

	//checks if bucket section is empty
	bool isBucketIndexEmpty(int index)
	{
		if (bucket[index] == nullptr)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//checks if hash table is empty
	bool isEmpty()
	{
		for (int i = 0; i < BUCKET_SIZE; i++)
		{
			//if bucket index isn't empty it returns false
			if (bucket[i] == nullptr)
			{
				
			}
			else
			{
				return false;
			}
		}
		return true;//if all bucket indexes are empty it returns true
	}

	//inserts new customer into hash table
	void insert(Customer &newPayload)
	{
		//declares section and the hash value
		int section;
		section = newPayload.getID() % BUCKET_SIZE;

		if (newPayload.getName() == "" || newPayload.getName().length() < 1)
		{
			return;
		}

		//if index is empty, populates it with a new node
		if (bucket[section] == nullptr)
		{
			Node * newNode = new Node();
			newNode->payload = newPayload;
			newNode->Point = nullptr;
			bucket[section] = newNode;
		}
		else
		{
			//declares and populates new node
			Node * newNode = new Node();
			newNode->payload = newPayload;
			newNode->Point = nullptr;

			//if the first items pointer is null, it populates it
			if (bucket[section]->Point == nullptr)
			{
				bucket[section]->Point = newNode;
			}
			else//if the first item has a pointer to something, it proceeds down the line of items in that 
			{//linked list

				//sets new traversal node to value after first item
				Node* traversal = bucket[section]->Point;
				while (traversal != nullptr)
				{
					if (traversal->Point == nullptr)
					{
						traversal = newNode;
					}
					traversal = traversal->Point;
				}
			}
		}

	}

	//returns customer reference of node
	Customer * getNode(int ID)
	{
		//determines which index to start from
		Node* traversal = bucket[ID%BUCKET_SIZE];

		//traverses to the end of the list
		while (traversal != nullptr)
		{
			//looks for node with ID of ID
			if (traversal->payload.getID() == ID)
			{
				return &(traversal->payload);
			}
			else
			{
				traversal = traversal->Point;
			}
		}

		//ouputs failure to fin
		std::cout << "Member not found\n";
		return nullptr;
	}

	//displays information of member
	void printNode(int ID)
	{
		//grabs pointer of index
		Node* traversal = bucket[ID%BUCKET_SIZE];
		//\
		I discovered this, just now, pretty dope \
		You\ can't place it inside the sentence or it doesn't work\
		runs while traversal is not nullptr
		while (traversal != nullptr)
		{
			//checks id of member, returns if matched
			if (traversal->payload.getID() == ID)
			{
				return traversal->payload.toString();
			}
			else
			{
				traversal = traversal->Point;
			}
		}

		std::cout << "Node not found\n";
	}

	//deletes last member in a index linked list
	Customer deleteEnd(int ID)//ID actually represents the bucket index
	{
		//gets proper bucket index pointer
		Node* traversal = bucket[ID%BUCKET_SIZE];

		//declares a dummy to be return in case of failure
		Customer dummy;
		dummy.setID(-767);

		//if it is the last node before nullptr, deletes info from node
		if (bucket[ID%BUCKET_SIZE]->Point == nullptr)
		{
			Customer returnPay = bucket[ID%BUCKET_SIZE]->payload;
			bucket[ID%BUCKET_SIZE]->payload.~Customer();
			bucket[ID%BUCKET_SIZE] = nullptr;
			return returnPay;
		}

		//traverses through list
		while (traversal != nullptr)
		{
			//if the last node before nullptr, deletes it
			if (traversal->Point == nullptr)
			{
				Customer returnPay = traversal->payload;
				traversal->payload.~Customer();
				traversal = nullptr;
				return returnPay;
			}
			else
			{
				traversal = traversal->Point;
			}
		}
		return dummy;// returns dummy if nothing is there, I wasn't sure how else to do this without getting the\
		not all control paths return a value
	}

	//returns bucket size
	int getBucketSize()
	{
		return BUCKET_SIZE;
	}

};
