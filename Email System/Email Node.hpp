#ifndef EMAILNODE
#define EMAILNODE

#include <iostream>

using namespace std;

struct EmailNode {
	string emailID;
	string email;
	string date;
	string priority;
	string subject;
	string content;
	EmailNode* next = nullptr;
};

#endif // !EMAILNODe
