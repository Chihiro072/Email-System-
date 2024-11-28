#include <iostream>
#include <iomanip>
#include "Stack.hpp"

using namespace std;

Stack::Stack(){
    top = bottom = nullptr;
    size = 0;
}

EmailNode* Stack::createNewNode(string emailID, string email, string date, string priority, string subject, string content) {

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

bool Stack::isEmpty() {

    if (top == nullptr)
        return true;
    return false;
}

void Stack::push(string emailID, string email, string date, string priority, string subject, string content) {
    
    EmailNode* newNode = createNewNode(emailID, email, date, priority, subject, content);

    if (isEmpty()) {
        top = bottom = newNode;
    }
    else {
        newNode->next = top;
        top = newNode;
    }
    size++;
}

void Stack::pop() {

    if (isEmpty())
        return;
    else {
        EmailNode* temp = top;
        top = temp->next;
        delete temp;
        size--;

        if (isEmpty())
            bottom = nullptr;
    }
}

EmailNode* Stack::peek() {
    return top;
}

void Stack::displayStack() {

    if (isEmpty())
        return;

    EmailNode* temp = top;
    while (temp != nullptr) {
        cout << left << setfill(' ') << setw(10) << temp->emailID << setw(25) << temp->email << setw(40) << temp->subject << setw(10) << temp->priority << setw(10) << temp->date << endl;
        temp = temp->next;
    }
}

int Stack::getSize() {
    return size;
}

void Stack::viewEmail(string emailID) {

    Stack tempStack;
    bool found = false;

    while (!isEmpty()) {
        EmailNode* top = peek();
        if (top->emailID == emailID) {
            system("cls");
            cout << "Subject: " << top->subject << endl;
            cout << "Content: " << endl;
            cout << top->content << endl;
            found = true;
            break;
        }
        tempStack.push(top->emailID, top->email, top->date, top->priority, top->subject, top->content);
        pop();
    }

    while (!tempStack.isEmpty()) {
        EmailNode* top = tempStack.peek();
        push(top->emailID, top->email, top->date, top->priority, top->subject, top->content);
        tempStack.pop();
    }

    if (!found) {
        cout << "Email not found." << endl;
    }
}

bool Stack::removeEmail(string emailID) {

    // remove email in stack sort by latest date
    Stack tempStack;
    int size = getSize();

    while (!isEmpty()) {
        EmailNode* top = peek();
        if (top->emailID != emailID) {
            tempStack.push(top->emailID, top->email, top->date, top->priority, top->subject, top->content);
        }
        pop();
    }

    // push email in temp stack back to email stack
    while (!tempStack.isEmpty()) {
        EmailNode* top = tempStack.peek();
        push(top->emailID, top->email, top->date, top->priority, top->subject, top->content);
        tempStack.pop();
    }

    if (size == getSize()) {
        return false;
    }
    return true;
}