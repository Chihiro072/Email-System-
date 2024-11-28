#ifndef STACK_HPP
#define STACK_HPP

#include <string>
#include "Email Node.hpp"

using namespace std;

class Stack {
private:
    EmailNode* top = nullptr;
    EmailNode* bottom = nullptr;
    int size = 0;
public:
    Stack();
    bool isEmpty();
    EmailNode* createNewNode(string emailID, string email, string date, string priority, string subject, string content);
    void push(string emailID, string email, string date, string priority, string subject, string content);
    void pop();
    EmailNode* peek();
    void displayStack();
    int getSize();
    void viewEmail(string emailID);
    bool removeEmail(string emailID);
};

#endif // STACK_HPP
