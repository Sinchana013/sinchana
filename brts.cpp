#include <iostream>
#include <string>
#include <ctime>
#include <queue>
#include <cstdlib>
#include <sstream>
#include <algorithm> // For transforming input to lowercase

using namespace std;

// Structure for a Ticket
struct Ticket {
    int id;
    string dateTime;   // Static date and time for simplicity
    string source;
    string destination;
    string status;     // "scanned-in", "scanned-out", or "not-scanned"
    double price;
};

// Structure for a Pass
struct Pass {
    int passId;
    string validityDate; // Format: YYYY-MM-DD
};

// Node for the BST
struct BSTNode {
    Ticket ticket;
    BSTNode* left;
    BSTNode* right;

    BSTNode(Ticket t) : ticket(t), left(NULL), right(NULL) {}
};

// Queue for Tickets and Passes
queue<Ticket> ticketQueue;
queue<Pass> passQueue;

// BST Root
BSTNode* root = NULL;

// Static Date and Time (Example)
string staticDateTime = "2025-01-02";  // Example static date

// Add Ticket to BST
BSTNode* insertIntoBST(BSTNode* root, Ticket ticket) {
    if (!root) return new BSTNode(ticket);
    if (ticket.id < root->ticket.id)
        root->left = insertIntoBST(root->left, ticket);
    else if (ticket.id > root->ticket.id)
        root->right = insertIntoBST(root->right, ticket);
    return root;
}

// Search in BST
BSTNode* searchInBST(BSTNode* root, int id) {
    if (!root || root->ticket.id == id) return root;
    if (id < root->ticket.id)
        return searchInBST(root->left, id);
    return searchInBST(root->right, id);
}

// Delete from BST
BSTNode* deleteFromBST(BSTNode* root, int id) {
    if (!root) return root;
    if (id < root->ticket.id)
        root->left = deleteFromBST(root->left, id);
    else if (id > root->ticket.id)
        root->right = deleteFromBST(root->right, id);
    else {
        if (!root->left) {
            BSTNode* temp = root->right;
            delete root;
            return temp;
        } else if (!root->right) {
            BSTNode* temp = root->left;
            delete root;
            return temp;
        }

        BSTNode* temp = root->right;
        while (temp->left) temp = temp->left;
        root->ticket = temp->ticket;
        root->right = deleteFromBST(root->right, temp->ticket.id);
    }
    return root;
}

// Add a New Ticket to the Queue (with random ticket ID)
void buyTicket() {
    Ticket t;
    t.id = rand() % 10000 + 1000;  // Random Ticket ID between 1000 and 10999
    cout << "Your Ticket ID is: " << t.id << endl;
    t.source = "SourceA";
    t.destination = "DestinationB";
    t.dateTime = staticDateTime;  // Using static date
    t.status = "not-scanned";
    t.price = 50.0; // Example static price

    ticketQueue.push(t);
    cout << "Ticket purchased successfully!" << endl;
    cout << "Ticket ID: " << t.id << "\nSource: " << t.source
         << "\nDestination: " << t.destination << "\nPrice: $" << t.price << endl;
}

// Scan-in Functionality
void scanIn() {
    cout << "Are you scanning a Ticket or Pass? (ticket/pass): ";
    string type;
    cin >> type;

    // Convert input to lowercase for case insensitivity
    transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "ticket") {
        int id;
        cout << "Enter Ticket ID: ";
        cin >> id;

        // Search for the ticket in the BST
        BSTNode* existingNode = searchInBST(root, id);

        if (existingNode) {
            // If the ticket is found and it's already scanned-in
            if (existingNode->ticket.status == "scanned-in") {
                cout << "Ticket already scanned in. No entry allowed!" << endl;
            } else {
                // Ticket is found and was scanned-out, allow scanning in again
                existingNode->ticket.status = "scanned-in";  // Change status to "scanned-in"
                cout << "Ticket scanned in successfully. Entry allowed!" << endl;
            }
        } else {
            // Ticket not found in BST, allowing the ticket to be added and scanned in
            Ticket t;
            t.id = id;
            t.dateTime = staticDateTime;  // Static date
            t.source = "source";
            t.destination = "destination";
            t.status = "scanned-in";
            root = insertIntoBST(root, t);
            cout << "Ticket scanned in successfully. Entry allowed!" << endl;
        }
    } else if (type == "pass") {
        int passId;
        cout << "Enter Pass ID: ";
        cin >> passId;

        bool found = false;
        queue<Pass> tempQueue = passQueue;
        while (!tempQueue.empty()) {
            if (tempQueue.front().passId == passId) {
                found = true;
                break;
            }
            tempQueue.pop();
        }

        if (found) {
            cout << "Pass is valid. Entry allowed!" << endl;
        } else {
            cout << "Invalid pass. No entry allowed!" << endl;
        }
    } else {
        cout << "Invalid input. Try again." << endl;
    }
}

// Scan-out Functionality
void scanOut() {
    string type;
    cout << "Are you scanning a Ticket or Pass? (ticket/pass): ";
    cin >> type;

    // Convert input to lowercase for case insensitivity
    transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "ticket") {
        int id;
        cout << "Enter Ticket ID: ";
        cin >> id;

        BSTNode* node = searchInBST(root, id);
        if (node && node->ticket.status == "scanned-in") {
            cout << "Exit allowed. Ticket scanned out successfully." << endl;
            node->ticket.status = "scanned-out";  // Change status to scanned-out
        } else {
            cout << "Ticket not found or not scanned-in. Exit denied!" << endl;
        }
    } else if (type == "pass") {
        int passId;
        cout << "Enter Pass ID: ";
        cin >> passId;

        bool found = false;
        queue<Pass> tempQueue = passQueue;
        while (!tempQueue.empty()) {
            if (tempQueue.front().passId == passId) {
                found = true;
                break;
            }
            tempQueue.pop();
        }

        if (found) {
            cout << "Exit allowed. Pass scanned out successfully." << endl;
        } else {
            cout << "Invalid pass. Exit denied!" << endl;
        }
    } else {
        cout << "Invalid input. Try again." << endl;
    }
}

// Function to create a new pass with a validity date 6 months from today
void createNewPass() {
    Pass p;
    p.passId = rand() % 10000 + 1000;  // Random Pass ID between 1000 and 10999
    cout << "Your Pass ID is: " << p.passId << endl;

    // Get the current date and calculate 6 months ahead
    time_t now = time(0);
    tm* currentTime = localtime(&now);

    // Add 6 months to the current date
    currentTime->tm_mon += 6;  // Add 6 months
    mktime(currentTime); // Normalize the tm structure

    // Format the new validity date (YYYY-MM-DD)
    stringstream ss;
    ss << 1900 + currentTime->tm_year << "-"
       << 1 + currentTime->tm_mon << "-"
       << currentTime->tm_mday;

    p.validityDate = ss.str();

    passQueue.push(p);
    cout << "Pass created successfully with validity until: " << p.validityDate << endl;
}

// Initialize the queue with 5 default passes and show their details
void initializePassQueue() {
    for (int i = 0; i < 5; i++) {
        Pass p;
        p.passId = rand() % 10000 + 1000;  // Random Pass ID between 1000 and 10999

        // Get the current date and calculate 6 months ahead
        time_t now = time(0);
        tm* currentTime = localtime(&now);

        // Add 6 months to the current date
        currentTime->tm_mon += 6;  // Add 6 months
        mktime(currentTime); // Normalize the tm structure

        // Format the new validity date (YYYY-MM-DD)
        stringstream ss;
        ss << 1900 + currentTime->tm_year << "-"
           << 1 + currentTime->tm_mon << "-"
           << currentTime->tm_mday;

        p.validityDate = ss.str();

        passQueue.push(p);

        // Display pass details
        cout << "Pass ID: " << p.passId << ", Validity Date: " << p.validityDate << endl;
    }
    cout<<endl;

    cout << "notice above is the list of passes just for refference " << endl;

    cout<<endl;
}

// Main Function
void sinchu() {
    srand(time(0));  // Seed the random number generator

    // Initialize the pass queue with 5 default passes
    initializePassQueue();
cout<<"lets start scanning and updating your tickets and passes..... If it fails, let's blame the programmer, not you :)." <<endl;
cout<<endl;
    int choice;

    do {
        cout << "\n--- BRTS Ticket Management ---\n";
        cout << "1. Buy Ticket\n";
        cout << "2. Scan In\n";
        cout << "3. Scan Out\n";
        cout << "4. Create New Pass\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: cout<<endl;
                    buyTicket();
                    cout<<endl;
                    break;
            case 2: cout<<endl;
                    scanIn();
                    cout<<endl;
                    break;
            case 3: cout<<endl;
                    scanOut();
                    cout<<endl;
                    break;
            case 4: cout<<endl;
                     createNewPass();
                     cout<<endl;
                     break;
            case 5: cout<<endl;
                    cout << "Exiting program. Goodbye!" << endl;
                    cout<<endl;
                     break;
            default: cout << "Invalid choice. Try again." << endl;
        }
    } while (choice != 5);

    return 0;
}
