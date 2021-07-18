/*
Program Name: Mechanic Shop
Author: Levi George
Date Last Updated: 12/20/2019
Program Purpose: To act as an interface for a mechanic's autobody/autorepair shop managing their parts, and customers, those that are waiting to be helped
and those who currently have their car in a bay being worked on. It will allow workers to modify the customer's receipt.

*/
#pragma once
#include "Parts.h"
#include <iostream>

class BSTree
{

private:

	//Node definition
	template<class Type>
	struct Node {

		Type payload;//holds the data of the node
		Node<Type> * lPoint, *rPoint; //points to the children of the node

		Node()//constructor
		{
			lPoint = nullptr;
			rPoint = nullptr;
		}
		~Node()//destructor
		{
			//std::cout << "Node of " << payload.getCode() << " has been destroyed.\n";
		}
	};//end of node definition

	Node<Part> * root;//holds the pointer to the root node


public:
	//constructor and destructor
	BSTree()
	{
		root = nullptr;
	}
	~BSTree()
	{
		deleteTree(root);
		//std::cout << "Deleted Tree\n";
	}

	//returns rootNode pointer
	Node<Part>* rootNode()
	{
		return root;
	}

	//gets the greatest value of the node, for specific use of balancing tree, somewhat
	int getGreatest()
	{
		Node<Part> * traversal = root;
		while (traversal != nullptr)
		{
			if (traversal->rPoint == nullptr)
			{
				return traversal->payload.getCode();
			}
			traversal = traversal->rPoint;
		}
		return -1;
	}

	//checks if binary tree is empty
	bool isEmpty()
	{
		if (root == nullptr)
		{
			return true;
		}
		else
			return false;
	}

	//deletes the Tree node by node
	void deleteTree(Node<Part>*POINT)
	{
		Node<Part> * thisNode = POINT;
		Node<Part> * traversal = POINT;

		//if root is empty or current pointer is empty, it returns, to avoid memory access violations
		if (root == nullptr)
		{
			return;
		}
		else if (POINT == nullptr)
		{
			return;
		}
		
		//checks left half of the tree first
		if (POINT->lPoint != nullptr)
		{
			deleteTree(traversal->lPoint);
		}
		
		//checks right half last
		if (POINT->rPoint != nullptr)
		{
			deleteTree(traversal->rPoint);
			
		}
		
		//takes care of deleting the root node and other nodes
		if (thisNode == root)
		{
			thisNode->payload.~Part();
			delete &(thisNode->payload);
			root->lPoint = nullptr;
			root->rPoint = nullptr;
			root = nullptr;
		}
		else
		{
			thisNode->payload.~Part();
			thisNode->lPoint = nullptr;
			thisNode->rPoint = nullptr;
			delete &(thisNode->payload);
			return;
		}
		
	}
	//inserts nodes with loaded payloads
	void insertNode(Part * CUST, Node<Part> * POINT)
	{
		//creates new node and loads it.
		Node<Part> * newNode = new Node<Part>;
		newNode->payload = *CUST;


		//if the root pointer is empty this section of code will load a new node into it
		//and initilize it's pointers to nothing
		if (root == nullptr)
		{
			root = newNode;
			root->lPoint = nullptr;
			root->rPoint = nullptr;

			//std::cout << "Root Node Value Added\n";
		}
		else if (POINT->payload == newNode->payload)//If a payload alreay exists with the same id, it will cancel the addition
		{
			//std::cout << "Duplicate Part ID: " << newNode->payload.getCode() << ", Name: " << newNode->payload.getName() << ".... addition aborted.\n";
		}
		else if (POINT->payload > newNode->payload)//if the new node has a lower ID than the current node being examined
		{										   //then the current node's left child is examined
			if (POINT->lPoint == nullptr)//if the left child is empty then the new node is added in it's place
			{
				POINT->lPoint = newNode;
				POINT->lPoint->rPoint = nullptr;
				POINT->lPoint->lPoint = nullptr;
				//std::cout << "Node Value Added to left child of " << POINT->payload.getCode() << "\n";
			}
			else//if the left child is populated, then that node is passe to the insertCust function
			{
				insertNode(CUST, POINT->lPoint);
				//std::cout << "Left child of " << POINT->payload.getCode() << " has been traversed\n";
			}
		}
		else if (POINT->payload < newNode->payload)//if the new node has a greater ID than the current node being examined 
		{	//then the current node's right child is examined

			if (POINT->rPoint == nullptr)//If the right child pointer is empty then the new node is added in its places
			{
				POINT->rPoint = newNode;
				POINT->rPoint->rPoint = nullptr;
				POINT->rPoint->lPoint = nullptr;
				//std::cout << "Node Value Added to right child of " << POINT->payload.getCode() << "\n";
			}
			else//if the right child pointer is populated then it sends the pointer of that node to the next function call
			{
				insertNode(CUST, POINT->rPoint);
				//std::cout << "Right child of " << POINT->payload.getCode() << " has been traversed\n";
			}
		}

	}

	//access part at specific node based on ID
	Part * getNode(Node<Part>*POINT, int ID)
	{
		if (POINT->payload.getCode() == ID)//If the pointer's node has the same ID as what was passed
		{								 //then it will print the payload of that node
			return &(POINT->payload);
		}
		else if (POINT->payload.getCode() < ID)//If the node has an ID that is less than the searched for ID, then it will
		{									 //pass the right pointer to the recursive call of the function, if the right pointer is empty it will return an error message 
			if (POINT->rPoint == nullptr)
			{
				//std::cout << "Node not found, search abandoned.\n";
				return nullptr;
			}
			else
			{
				return getNode(POINT->rPoint, ID);
			}
		}
		else if (POINT->payload.getCode() > ID)//If the node has an ID that is greater than the searched for ID, then it will
		{									  //pass the right pointer to the recursive call of the function, if the left pointer is empty it will return an error message
			if (POINT->lPoint == nullptr)
			{
				//std::cout << "Node not found, search abandoned.\n";
				return nullptr;
			}
			else
			{
				return getNode(POINT->lPoint, ID);
			}
		}
		else
		{
			//std::cout << "Node not found, search abandoned.\n";//sends error message in other cases.
			return nullptr;
		}
		return nullptr;
	}

	//this function accepts a pointer and an ID and will find that ID in the binary tree
	void findNode(Node<Part>*POINT, int ID)
	{
		if (POINT->payload.getCode() == ID)//If the pointer's node has the same ID as what was passed
		{								 //then it will print the payload of that node
			POINT->payload.print();
			return ;
		}
		else if (POINT->payload.getCode() < ID)//If the node has an ID that is less than the searched for ID, then it will
		{									 //pass the right pointer to the recursive call of the function, if the right pointer is empty it will return an error message 
			if (POINT->rPoint == nullptr)
			{
			//	std::cout << "Node not found, search abandoned.\n";
				return;
			}
			findNode(POINT->rPoint, ID);
			return ;
		}
		else if (POINT->payload.getCode() > ID)//If the node has an ID that is greater than the searched for ID, then it will
		{									  //pass the right pointer to the recursive call of the function, if the left pointer is empty it will return an error message
			if (POINT->lPoint == nullptr)
			{
				//std::cout << "Node not found, search abandoned.\n";
				return ;
			}
			findNode(POINT->lPoint, ID);
			return ;
		}
		else
		{
			//std::cout << "Node not found, search abandoned.\n";//sends error message in other cases.
		}
	}

};
