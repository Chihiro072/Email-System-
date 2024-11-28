#include <thread>
#include <chrono>
#include "User.hpp"

User::User() {
    email = "";
    password = "";
}

string User::lowercaseString(string input) {
    string lowerCase;
    for (char c : input)
        lowerCase += tolower(c);
    return lowerCase;
}

void User::pauseForKeyPress() {
    cout << "Press Enter or any key to continue..." << endl;
    cin.ignore();
    cin.get();     // Wait for the user to press any key
}

bool User::authenticateUser() {
    userFile.open("User Email.csv"); // Open the file for reading
    string line, email, password;

    while (getline(userFile, line)) {
        // Check if the line is empty
        if (line.empty())
            continue;

        // Use getline to extract email and password from the line
        stringstream ss(line);
        getline(ss, email, ','); // Extract email
        getline(ss, password, ','); // Extract password

        // Skip header line if it contains "Email"
        if (email.find("Email") != string::npos) continue;

        // Compare emails in lowercase and passwords
        if (lowercaseString(email) == this->email && password == this->password) {
            userFile.close(); // Close the file
            return true;  // Login successful
        }
    }

    userFile.close(); // Close the file
    cout << "Login Failed" << endl;
    pauseForKeyPress();
    return false;
}

bool User::isDuplicate(string email, string content) {

    EmailNode* current = inboxStack.peek();
    // Traverse the stack to find duplicates
    while (current != nullptr) {
        if (current->email == email && current->content == content) {
            return true;
        }
        current = current->next;  // Move to the next node
    }
    return false;
}

bool User::isExceedLimit(string email, string date) {

    int count = 1;
    EmailNode* current = inboxStack.peek();

    while (current != nullptr) {
        // if same sender and same month and date
        if ((current->email == email) && (current->date.substr(0, 10) == date.substr(0, 10))) {
            count++;
            if (count >= 5) {
                return true;
            }
        }
        current = current->next;
    }
    return false;
}

void User::findEmail() {
    emailFile.open("Mail.csv");
    string line, emailID, sender, receiver, date, priority, subject, content;

    while (getline(emailFile, line)) {
        
        if (line.empty())
            continue;

        // Use getline to extract email and password from the line
        stringstream ss(line);
        getline(ss, emailID, ','); // Extract emailid
        getline(ss, sender, ','); // Extract sender
        getline(ss, receiver, ','); // Extract receiver
        getline(ss, date, ','); // Extract date
        getline(ss, priority, ','); // Extract priority
        getline(ss, subject, ','); // Extract subject
        getline(ss, content, ','); // Extract content

        // Skip header line if it contains "emailID"
        if (email.find("emailID") != string::npos)
            continue;

        // Determine is user sender or receiver
        if (lowercaseString(sender) == this->email) {
            // Add email to outbox sort by latest date and priority
            sentStack.push(emailID, "to: " + receiver, date, priority, subject, content);
            sentPriority.enqueueWithPriority(emailID, "to: " + receiver, date, priority, subject, content);
        }
        else if (lowercaseString(receiver) == this->email) {
            if (isExceedLimit(sender, date) || isDuplicate(sender, content)) {
                // Add to spam
                spamStack.push(emailID, sender, date, priority, subject, content);
            }
            else {
                // Add email to inbox sort by latest date and priority
                inboxStack.push(emailID, sender, date, priority, subject, content);
                inboxPriority.enqueueWithPriority(emailID, sender, date, priority, subject, content);
            }   
        }
    }
    emailFile.close();
}

void User::deleteEmail(string emailID) {
    // Open the original CSV file for reading
    ifstream emailInFile("Mail.csv");

    // Check if the file opened successfully
    if (!emailInFile.is_open()) {
        cout << "Error: Unable to open file." << endl;
        return;
    }

    // Create a temporary file to write the new content
    ofstream tempOutFile("TempMail.csv");

    // Check if the temporary file opened successfully
    if (!tempOutFile.is_open()) {
        cout << "Error: Unable to create temporary file." << endl;
        emailInFile.close();
        return;
    }

    string line;

    // Read the original file line by line
    while (getline(emailInFile, line)) {
        istringstream lineStream(line);
        string id;

        // Extract the emailID (the first field in the CSV line)
        getline(lineStream, id, ',');

        // If the emailID doesn't match the target, write the line to the temporary file
        if (id != emailID) {
            tempOutFile << line << endl;
        }
    }

    // Close both the original file and the temporary file
    emailInFile.close();
    tempOutFile.close();

    // Remove the original file and rename the temporary file to the original file
    if (remove("Mail.csv") != 0) {
        cout << "Error: Unable to remove original file." << endl;
        return;
    }

    if (rename("TempMail.csv", "Mail.csv") != 0) {
        cout << "Error: Unable to rename temporary file." << endl;
        return;
    }
}

void User::emptyStack(int choice, Stack& emailStack) {

    while (!emailStack.isEmpty()) {

        if (choice == 1) {
            deleteEmail(emailStack.peek()->emailID);
        }
        emailStack.pop();
    }
}

void User::askForEmailID(int choice, int subChoice) {

    string emailID;
    cout << endl;
    cout << "Please insert an Email ID: ";
    cin >> emailID;
    emailID[0] = toupper(emailID[0]);

    if (subChoice == 2) {
        if (choice == 1)
            inboxStack.viewEmail(emailID);
        else if (choice == 3)
            sentStack.viewEmail(emailID);
        else if (choice == 6)
            spamStack.viewEmail(emailID);
        else
            searchStack.viewEmail(emailID);
    }
    else if (subChoice == 3) {

        bool found = false;
        if (choice == 1) {
            found = inboxStack.removeEmail(emailID);
            inboxPriority.removeEmail(emailID);
        }
        else if (choice == 3) {
            found = sentStack.removeEmail(emailID);
            sentPriority.removeEmail(emailID);
        }
        else if (choice == 6)
            found = spamStack.removeEmail(emailID);

        if (found) {
            cout << "Email deleted successfully" << endl;
            deleteEmail(emailID);
        }
        else
            cout << "Email not found" << endl;
    }
    pauseForKeyPress();
    return;
}

void User::displaySubMenu(int choice) {

    int subChoice = 0;
    bool priority = false;

    do {
        system("cls");
        cout << setfill(' ') << setw(10) << "Email ID" << setw(25) << "Email" << setw(40) << "Subject" << setw(10) << "Priority" << setw(10) << "Date" << endl;

        if (choice == 1) {
            if (inboxStack.isEmpty()) {
                cout << "Inbox is empty." << endl;
                pauseForKeyPress();
                return;
            }
            if (priority) {
                inboxPriority.displayQueue();
                cout << "1. Sort by latest date" << endl;
            }
            else {
                inboxStack.displayStack();
                cout << "1. Sort by priority" << endl;
            }
        }
        else if (choice == 3) {
            if (sentPriority.isEmpty()) {
                cout << "Sent is empty." << endl;
                pauseForKeyPress();
                return;
            }
            if (priority) {
                sentPriority.displayQueue();
                cout << "1. Sort by latest date" << endl;
            }
            else {
                sentStack.displayStack();
                cout << "1. Sort by priority" << endl;
            }
        }
        else if (choice == 6) {
            if (spamStack.isEmpty()) {
                cout << "Spam is empty." << endl;
                pauseForKeyPress();
                return;
            }
            spamStack.displayStack();
            cout << "1. Empty spam" << endl;
        }
        cout << "2. View Email" << endl;
        cout << "3. Delete Email" << endl;
        cout << "4. Back" << endl;
        cout << "Enter your choice: ";

        if (!(cin >> subChoice) || subChoice < 1 || subChoice > 4) { // Check if input is not a number or out of range
            cout << "Invalid input." << endl;
            cin.clear(); // Clear the error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            pauseForKeyPress();
            subChoice = 0; // Reset choice to stay in loop
        }
        else {
            if (subChoice == 1) {
                if (choice == 1 || choice == 3)
                    if (priority)
                        priority = false;
                    else
                        priority = true;
                else if (choice == 6) {
                    emptyStack(subChoice, spamStack);
                }
            }
            else if (subChoice == 2 || subChoice == 3)
                askForEmailID(choice, subChoice);
            else if (subChoice == 4)
                return;
        }
    } while (subChoice >= 0 && subChoice <= 4);
}

string User::generateEmailID() {
    ifstream emailInFile("Mail.csv");
    string line;
    string lastID = "E000";  // Starting point if no IDs exist

    // Get last ID
    while (getline(emailInFile, line)) {
        if (!line.empty()) {
            stringstream ss(line);
            getline(ss, lastID, ',');
        }
    }
    emailInFile.close();

    // Extract number from the emailID
    int numberID = stoi(lastID.substr(1)) + 1;
    stringstream newID;
    newID << 'E' << setw(3) << setfill('0') << numberID;

    return newID.str();
}

void User::sendEmail() {

    // Delay for 30 seconds
    std::this_thread::sleep_for(std::chrono::seconds(30));

    ofstream emailOutFile("Mail.csv", ios::app);

    // Transfer email from outbox to sent after delay
    while (!outboxQueue.isEmpty()) {
        EmailNode* front = outboxQueue.peek();
        emailOutFile << front->emailID << "," << this->email << "," << front->email << "," << front->date << "," << front->priority << "," << front->subject << "," << front->content << endl;
        sentStack.push(front->emailID, "to: " + front->email, front->date, front->priority, front->subject, front->content);
        sentPriority.enqueueWithPriority(front->emailID, "to: " + front->email, front->date, front->priority, front->subject, front->content);
        outboxQueue.dequeue();
    }
    emailOutFile.close();
}

void User::addToOutbox() {
    emailFile.open("Mail.csv");
    string receiver, subject, content, priority;

    // Input and validate recipient email
    while (!checkEmail(receiver)) {
        cout << "Enter the recipient's email: ";
        cin >> receiver;

        if (!checkEmail(receiver)) {
            cout << "Recipient not found. Please try again." << endl;
        }
    }

    emailFile.close();

    // Input for subject and content
    cout << "Enter the subject: ";
    cin.ignore();
    getline(cin, subject);

    cout << "Enter the content: ";
    getline(cin, content);

    // Input for priority
    while (true) {
        cout << "Enter priority (low, medium, high): ";
        cin >> priority;

        lowercaseString(priority);

        // Capitalize the first letter
        priority[0] = toupper(priority[0]);

        if (priority == "Low" || priority == "Medium" || priority == "High") {
            break;
        }
        else {
            cout << "Invalid priority. Please enter 'Low', 'Medium', or 'High'." << endl;
        }
    }

    // Get current date and time
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);
    stringstream date;
    date << setw(2) << setfill('0') << (1 + ltm.tm_mon) << "/"
        << setw(2) << setfill('0') << ltm.tm_mday << "/"
        << (1900 + ltm.tm_year) << " "
        << setw(2) << setfill('0') << ltm.tm_hour << ":"
        << setw(2) << setfill('0') << ltm.tm_min;

    // Add to Outbox Queue
    outboxQueue.enqueue(generateEmailID(), receiver, date.str(), priority, subject, content);

    cout << "Email will be sent within 30 seconds..." << endl;
    pauseForKeyPress();

    std::thread([this]() {
        this->sendEmail();
        }).detach();

}

void User::searchEmail(string keyword, Stack& emailStack) {

    keyword = lowercaseString(keyword);
    Stack tempStack;

    while (!emailStack.isEmpty()) {
        EmailNode* top = emailStack.peek();
        string emailID = lowercaseString(top->emailID);
        string email = lowercaseString(top->email);
        string date = lowercaseString(top->date);
        string priority = lowercaseString(top->priority);
        string subject = lowercaseString(top->subject);
        string content = lowercaseString(top->content);
        if (top->emailID.find(keyword) != string::npos || top->email.find(keyword) != string::npos || top->date.find(keyword) != string::npos || top->priority.find(keyword) != string::npos || top->subject.find(keyword) != string::npos || top->content.find(keyword) != string::npos) {
            searchStack.push(top->emailID, top->email, top->date, top->priority, top->subject, top->content);
        }
        tempStack.push(top->emailID, top->email, top->date, top->priority, top->subject, top->content);
        emailStack.pop();
    }

    while (!tempStack.isEmpty()) {
        EmailNode* top = tempStack.peek();
        emailStack.push(top->emailID, top->email, top->date, top->priority, top->subject, top->content);
        tempStack.pop();
    }
}

void User::displayMenu() {

    int choice = 0;

    do {
        system("cls");
        cout << left << setw(40) << setfill('=') << '=' << endl;
        cout << left << setw(10) << setfill(' ') << "Welcome, " << email << endl;
        cout << left << setw(40) << setfill('=') << '=' << endl;
        cout << endl;
        cout << "1. Inbox" << endl;
        cout << "2. Send Email" << endl;
        cout << "3. Sent" << endl;
        cout << "4. Outbox" << endl;
        cout << "5. Search" << endl;
        cout << "6. Spam" << endl;
        cout << "0. Logout" << endl;
        cout << "Enter your choice: ";

        if (!(cin >> choice)) { // Check if input is not a number
            cout << "Invalid input." << endl;
            pauseForKeyPress();
            cin.clear(); // Clear the error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            choice = 1; // Reset choice to stay in loop
        }

        system("cls");

        if (choice == 0) {
            return;
        }
        else if (choice == 1 || choice == 3 || choice == 6) {
            displaySubMenu(choice);
        }
        else if (choice == 2) {
            addToOutbox();
        }
        else if (choice == 4) {
            if (outboxQueue.isEmpty()) {
                cout << "Outbox is empty." << endl;
            }
            else {
                cout << setfill(' ') << setw(10) << "Email ID" << setw(25) << "Email" << setw(40) << "Subject" << setw(10) << "Priority" << setw(10) << "Date" << endl;
                outboxQueue.displayQueue();
            }
            pauseForKeyPress();
        }
        else if (choice == 5) {
            string keyword = "";
            cout << "Enter keyword to search: ";
            cin >> keyword;
            searchEmail(keyword, inboxStack);
            searchEmail(keyword, sentStack);
            searchEmail(keyword, spamStack);
            if (searchStack.isEmpty()) {
                cout << "Keyword does not match any email" << endl;
                pauseForKeyPress();
            }
            else {
                cout << endl;
                cout << setfill(' ') << setw(10) << "Email ID" << setw(25) << "Email" << setw(40) << "Subject" << setw(10) << "Priority" << setw(10) << "Date" << endl;
                searchStack.displayStack();
                askForEmailID(choice, 2);
                emptyStack(choice, searchStack);
            }
        }
        else {
            cout << "Invalid input." << endl;
            choice = 1;
            pauseForKeyPress();
        } 

    } while (choice >= 0 && choice <= 6);
    

}

void User::login() {
    
    do {
        system("cls");
        cout << left << setw(40) << setfill('=') << '=' << endl;
        cout << left << setw(14) << setfill(' ') << ' ' << "L O G I N" << endl;
        cout << left << setw(40) << setfill('=') << '=' << endl;
        cout << endl;
        cout << "Email: ";
        cin >> this->email;
        cout << "Password: ";
        cin >> this->password;

        this->email = lowercaseString(this->email);

    } while (!authenticateUser());
    this->email = "john@example.com";
    findEmail();
    displayMenu();
}

bool User::checkEmail(string registerEmail) {
    userFile.open("User Email.csv");
    userFile.clear();
    userFile.seekg(0);
    string line, email;

    while (getline(userFile, line)) {
        if (line.empty())
            continue;

        // Use getline to extract email from the line
        stringstream ss(line);
        getline(ss, email, ','); // Extract email

        // Skip header line if it contains "Email"
        if (email.find("Email") != string::npos) 
            continue;

        // Check if the input matches the credentials
        if (lowercaseString(email) == lowercaseString(registerEmail)) {
            userFile.close();
            return true;  // Email is already registered
        }
    }
    userFile.close();
    return false;  // Email is not registered
}

void User::registerUser() {
    
    bool registration = true;
    do {
        registration = false;
        system("cls");
        cout << left << setw(60) << setfill('=') << '=' << endl;
        cout << left << setw(20) << setfill(' ') << ' ' << "R E G I S T E R" << endl;
        cout << left << setw(60) << setfill('=') << '=' << endl << endl;
        userOutputFile.open("User Email.csv", ios::app);

        string registerEmail, registerPassword;
        cout << "Please enter your email: ";
        cin >> registerEmail;

        if (checkEmail(registerEmail)) {
            cout << "The email has been registered!" << endl;
            pauseForKeyPress();
            registration = true;
            continue;
        }

        cout << "Please enter your password: ";
        cin >> registerPassword;

        userOutputFile << lowercaseString(registerEmail) << "," << registerPassword << endl;
        userOutputFile.close();

        cout << "Registration successful!" << endl;
    } while (registration);
}