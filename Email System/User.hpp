#ifndef USER_HPP
#define USER_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include "Stack.hpp"
#include "Queue.hpp"

using namespace std;

class User {
private:
	string email;
	string password;
	ifstream userFile;
	ifstream emailFile;
	ofstream userOutputFile;
	ofstream emailOutputFile;
	Stack inboxStack;
	Stack sentStack;
	Stack spamStack;
	Stack searchStack;
	Queue inboxPriority;
	Queue sentPriority;
	Queue outboxQueue;
public:
	User();
	string lowercaseString(string email);
	void pauseForKeyPress();
	bool authenticateUser();
	bool isDuplicate(string email, string content);
	bool isExceedLimit(string email, string content);
	void findEmail();
	void deleteEmail(string emailID);
	void emptyStack(int choice, Stack& emailStack);
	void askForEmailID(int choice, int subChoice);
	void displaySubMenu(int choice);
	string generateEmailID();
	void sendEmail();
	void addToOutbox();
	void searchEmail(string keyword, Stack& emailStack);
	void displayMenu();
	void login();
	bool checkEmail(string email);
	void registerUser();
};

#endif