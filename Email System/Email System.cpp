#include <iostream>
#include "User.hpp"

int main()
{
    User user;
    char ownAccount;
    do {
        cout << "Do you have an account (y/n): ";
        cin >> ownAccount;
        system("cls");
        if (tolower(ownAccount) == 'y')
            user.login();
        else if (tolower(ownAccount) == 'n')
            user.registerUser();
        else
            cout << "Invalid input, please enter 'y' or 'n'." << endl;
    } while (ownAccount != 'y' && ownAccount != 'n');
}