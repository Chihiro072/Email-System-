#ifndef QUEUE
#define QUEUE

#include <iostream>
#include "Email Node.hpp"

using namespace std;

class Queue {
private:
	EmailNode* front = nullptr;
	EmailNode* rear = nullptr;
	int size = 0;
public:
	Queue();
	EmailNode* createNewNode(string emailID, string email, string date, string priority, string subject, string content);
	bool isEmpty();
	bool comparePriority(string first, string second);
	void enqueue(string emailID, string email, string date, string priority, string subject, string content);
	void enqueueWithPriority(string emailID, string email, string date, string priority, string subject, string content);
	void dequeue();
	EmailNode* peek();
	void displayQueue();
	int getSize();
	void removeEmail(string emailID);
};

#endif