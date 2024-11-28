#include <iomanip>
#include "Queue.hpp"

using namespace std;

Queue::Queue() {
	front = rear = nullptr;
	size = 0;
}

EmailNode* Queue::createNewNode(string emailID, string email, string date, string priority, string subject, string content) {
	EmailNode* newNode = new EmailNode;
	newNode->emailID = emailID;
	newNode->email = email;
	newNode->date = date;
	newNode->priority = priority;
	newNode->subject = subject;
	newNode->content = content;
	newNode->next = nullptr;

	return newNode;
}

bool Queue::isEmpty() {
	
	if (front == nullptr)
		return true;
	return false;
}

bool Queue::comparePriority(string first, string second) {

	if (first == second)
		return false;
	else if (first == "High")
		return true;
	else if (first == "Medium" && second == "Low")
		return true;
	return false;
}

void Queue::enqueue(string emailID, string email, string date, string priority, string subject, string content) {

	EmailNode* newNode = createNewNode(emailID, email, date, priority, subject, content);

	if (isEmpty()) {
		front = rear = newNode;
	}
	else {
		rear->next = newNode;
		rear = newNode;
	}
	size++;
}

void Queue::enqueueWithPriority(string emailID, string email, string date, string priority, string subject, string content) {

	EmailNode* newNode = createNewNode(emailID, email, date, priority, subject, content);

	if (isEmpty()) {
		front = rear = newNode;
	}
	else if (comparePriority(newNode->priority, front->priority)) {
		newNode->next = front;
		front = newNode;
	}
	else if (newNode->priority == "Low") {
		rear->next = newNode;
		rear = newNode;
	}
	else {
		EmailNode* previous = front;
		EmailNode* current = previous->next;
		while (current != nullptr) {
			if (comparePriority(newNode->priority, current->priority)) {
				break;
			}
			previous = current;
			current = current->next;
		}
		newNode->next = current;
		previous->next = newNode;
		if (current == nullptr)
			rear = newNode;
	}
	size++;
}

void Queue::dequeue() {

	if (isEmpty())
		return;
	else {
		EmailNode* temp = front;
		front = temp->next;
		delete temp;
		size--;

		if (isEmpty())
			rear = nullptr;
	}
}

EmailNode* Queue::peek() {
	return front;
}

void Queue::displayQueue() {
	
	EmailNode* temp = front;
	while (temp != nullptr) {
		cout << left << setw(10) << temp->emailID << setw(25) << temp->email << setw(40) << temp->subject << setw(10) << temp->priority << setw(10) << temp->date << endl;
		temp = temp->next;
	}
}

int Queue::getSize() {
	return size;
}

void Queue::removeEmail(string emailID) {
	
	for (int i = 0; i < getSize(); i++) {
		// remove email in priority queue sort by priority
		EmailNode* front = peek();
		if (front->emailID != emailID) {
			enqueue(front->emailID, front->email, front->date, front->priority, front->subject, front->content);
		}
		dequeue();
	}
}