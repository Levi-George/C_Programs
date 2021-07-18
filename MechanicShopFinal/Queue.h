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
#include "Customer.h"

//template <class T>
class Queue
{
private:
	//This node represents the smaller data structures used to help navigate a linked list
	struct Node
	{
		Customer payload; //In this case a character
		Node * next;
	};

	//This is the last node stored within the stack
	Node *Top;

public:
	//constructor
	Queue()
	{
		Top = nullptr;
	}
	//destructor
	~Queue()
	{
		destructStack();
	}

	//checks if queue is empty
	bool isEmpty()
	{
		if (Top == nullptr)
		{
			return true;
		}
		else
			return false;
	}

	//checks if there is a node in the first position
	Customer* Peek()
	{
		//declares and initializes node
		Node *traversalNode;
		traversalNode = Top;

		//checks if the top node is a nullptr
		if (Top == nullptr)
		{
			//std::cout << "No nodes left\n";
			return nullptr;
		}
		else//returns if there is something in the queue
		{
			//std::cout << "Node returned\n";
			return &(Top->payload);
		}
	}
	//deallocates all memory and clears stacks attributes to the baseline
	void destructStack()
	{
		while (Top != nullptr)
		{
			Node *Header;

			Header = Top;
			Top = Top->next;

			delete Header;
		}
	}
	//puts a new stack item at the back of the queue
	void Enqueue(Customer * newPack)
	{
		//declares new node and allocates memory
		Node *newNode = new Node();
		Node *traversalNode;

		traversalNode = Top;
		newNode->payload = *newPack;
		newNode->next = nullptr;

		if (Top == nullptr)
		{
			Top = newNode;
		}
		else if (Top->next == nullptr)
		{
			Top->next = newNode;
		}
		else
		{
			while (traversalNode != nullptr)
			{
				if (traversalNode == nullptr)
				{
					traversalNode = newNode;
				}
				else
				{
					traversalNode = traversalNode->next;
				}
			}
		}

	}

	//removes node from the node of the stack
	Customer dequeue()
	{
		//declares removedNode pointer and payload package
		Node *removedNode = new Node();
		Customer dummy;
		Customer payload;

		if (Top != nullptr)
		{
			//rearranges nodes
			removedNode = Top;
			Top = Top->next;
			payload = removedNode->payload;

			//deallocates memory
			delete removedNode;

			//returns function
			return payload;
		}
		else
		{
			std::cout << "Queue empty\n";
			return dummy;
		}

	}
};
