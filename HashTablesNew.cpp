//============================================================================
// Name        : HashTable.cpp
// Author      : Jorgo Qendro
// Version     : 1.0
// Copyright   : Copyright © 2023 SNHU COCE
// Description : Lab 4-2 Hash Table
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <time.h>
#include <vector> // For vector usage
#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
    // Define structures to hold bids
    struct Node {
        Bid bid;
        unsigned int key;
        Node* next;

        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        // initialize with a bid
        Node(Bid aBid) : Node() {
            bid = aBid;
        }

        // initialize with a bid and a key
        Node(Bid aBid, unsigned int aKey) : Node(aBid) {
            key = aKey;
        }
    };

    vector<Node> nodes;

    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string bidId);
    size_t Size();  // Define Size function here
};

/**
 * Default constructor
 */
HashTable::HashTable() {
    // Initialize the structure by resizing nodes to tableSize
    nodes.resize(tableSize);
}

/**
 * Constructor for specifying size of the table
 */
HashTable::HashTable(unsigned int size) {
    this->tableSize = size;
    nodes.resize(tableSize);
}

/**
 * Destructor
 */
HashTable::~HashTable() {
    // Clean up the nodes
    nodes.clear();
}

/**
 * Calculate the hash value of a given key.
 */
unsigned int HashTable::hash(int key) {
    return key % tableSize;
}

/**
 * Insert a bid
 */
void HashTable::Insert(Bid bid) {
    // Create the key for the given bid
    unsigned int key = hash(atoi(bid.bidId.c_str()));

    // Retrieve node using key
    Node* prev = nullptr;
    Node* node = &nodes.at(key);

    // If no entry found for the key
    if (node->key == UINT_MAX) {
        node->key = key;
        node->bid = bid;
    }
    // Else find the next open node
    else {
        while (node != nullptr) {
            prev = node;
            node = node->next;
        }
        // Add new node to the end
        prev->next = new Node(bid, key);
    }
}

/**
 * Print all bids
 */
void HashTable::PrintAll() {
    for (unsigned int i = 0; i < tableSize; ++i) {
        Node* node = &nodes.at(i);
        if (node->key != UINT_MAX) {
            cout << node->key << ": " << node->bid.bidId << " | " << node->bid.title << " | "
                << node->bid.amount << " | " << node->bid.fund << endl;
            node = node->next;
            while (node != nullptr) {
                cout << node->key << ": " << node->bid.bidId << " | " << node->bid.title << " | "
                    << node->bid.amount << " | " << node->bid.fund << endl;
                node = node->next;
            }
        }
    }
}

/**
 * Remove a bid
 */
void HashTable::Remove(string bidId) {
    unsigned int key = hash(atoi(bidId.c_str()));
    Node* node = &nodes.at(key);
    Node* prev = nullptr;

    // Traverse to find the node to remove
    while (node != nullptr && node->bid.bidId != bidId) {
        prev = node;
        node = node->next;
    }

    // Node not found
    if (node == nullptr) {
        return;
    }

    // Node found - update pointers
    if (prev == nullptr) {
        // Removing head node
        nodes.at(key).key = UINT_MAX;
    }
    else {
        prev->next = node->next;
    }

    delete node;
}

/**
 * Search for the specified bidId
 */
Bid HashTable::Search(string bidId) {
    unsigned int key = hash(atoi(bidId.c_str()));
    Node* node = &nodes.at(key);

    // Traverse to find the bid
    while (node != nullptr && node->bid.bidId != bidId) {
        node = node->next;
    }

    if (node != nullptr) {
        return node->bid;
    }

    return Bid();  // Return empty bid if not found
}

/**
 * Get the size of the hash table
 */
size_t HashTable::Size() {
    return nodes.size();
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
        << bid.fund << endl;
}

/**
 * Load a CSV file containing bids into a container
 */
void loadBids(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    // Initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // Read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // Loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            hashTable->Insert(bid);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Convert a string to a double after stripping out unwanted char
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {
    string csvPath, bidKey;

    // Fix: Initialize bidKey outside the switch block to avoid skipping initialization
    Bid bid;

    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98223";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv";
        bidKey = "98223";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a hash table to hold all the bids
    HashTable* bidTable = new HashTable();

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "   3. Find Bid" << endl;
        cout << "   4. Remove Bid" << endl;
        cout << "   9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            // Initialize bidKey before use
            bidKey = "98223";

            // Initialize start time
            ticks = clock();

            // Load bids from a CSV file
            loadBids(csvPath, bidTable);

            // Calculate elapsed time and display result
            ticks = clock() - ticks;
            cout << "Time: " << ticks << " milliseconds" << endl;
            cout << "Time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bidTable->PrintAll();
            break;

        case 3:
            ticks = clock();

            bid = bidTable->Search(bidKey);

            ticks = clock() - ticks;

            if (!bid.bidId.empty()) {
                displayBid(bid);
            }
            else {
                cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "Time: " << ticks << " clock ticks" << endl;
            cout << "Time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 4:
            bidTable->Remove(bidKey);
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
