//============================================================================
// Name        : VectorSorting.cpp
// Author      : Jorgo Qendro
// Version     : 1.0
// Copyright   : Copyright © 2023 SNHU COCE
// Description : Vector Sorting Algorithms
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

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
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
        << bid.fund << endl;
    return;
}

/**
 * Prompt user for bid information using console (std::in)
 *
 * @return Bid struct containing the bid info
 */
Bid getBid() {
    Bid bid;

    cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);

    cout << "Enter title: ";
    getline(cin, bid.title);

    cout << "Enter fund: ";
    cin >> bid.fund;

    cout << "Enter amount: ";
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
vector<Bid> loadBids(string csvPath) {
    cout << "Loading CSV file " << csvPath << endl;

    // Define a vector data structure to hold a collection of bids.
    vector<Bid> bids;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            // push this bid to the end
            bids.push_back(bid);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
    return bids;
}

//============================================================================
// Sorting Algorithms
//============================================================================

/**
 * Perform a selection sort on bid title
 * Average performance: O(n^2))
 * Worst case performance O(n^2))
 *
 * @param bid address of the vector<Bid>
 *            instance to be sorted
 */
void selectionSort(vector<Bid>& bids) {
    size_t size = bids.size();

    // Iterate over each element in the vector
    for (size_t pos = 0; pos < size - 1; ++pos) {
        // Assume the current position is the minimum
        size_t min = pos;

        // Iterate over the unsorted portion of the vector
        for (size_t j = pos + 1; j < size; ++j) {
            // If the current element is less than the current minimum, update min
            if (bids[j].title < bids[min].title) {
                min = j;
            }
        }

        // Swap the current element with the found minimum
        if (min != pos) {
            swap(bids[pos], bids[min]);
        }
    }
}

/**
 * Partition the vector of bids into two parts, low and high
 *
 * @param bids Address of the vector<Bid> instance to be partitioned
 * @param begin Beginning index to partition
 * @param end Ending index to partition
 */
int partition(vector<Bid>& bids, int begin, int end) {
    // Select the pivot (here we choose the last element as pivot)
    string pivot = bids[end].title;

    // Pointer for the greater element
    int low = begin - 1;

    // Traverse the vector and compare each element with the pivot
    for (int j = begin; j < end; ++j) {
        if (bids[j].title <= pivot) {
            // If element smaller than pivot is found, swap it with the greater element pointed by low
            low++;
            swap(bids[low], bids[j]);
        }
    }

    // Swap the pivot element with the element at low+1 (so that pivot comes to its correct sorted position)
    swap(bids[low + 1], bids[end]);

    // Return the partition index
    return low + 1;
}

/**
 * Perform a quick sort on bid title
 * Average performance: O(n log(n))
 * Worst case performance O(n^2))
 *
 * @param bids address of the vector<Bid> instance to be sorted
 * @param begin the beginning index to sort on
 * @param end the ending index to sort on
 */
void quickSort(vector<Bid>& bids, int begin, int end) {
    if (begin >= end) {
        return;  // Base case: if only one element or none, return
    }

    // Partition the bids and get the partition index
    int mid = partition(bids, begin, end);

    // Recursively apply quicksort to the left and right partitions
    quickSort(bids, begin, mid - 1);  // Sort the left partition
    quickSort(bids, mid + 1, end);    // Sort the right partition
}

//============================================================================
// Utility Functions
//============================================================================

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

//============================================================================
// Main Method
//============================================================================

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv";
    }

    // Define a vector to hold all the bids
    vector<Bid> bids;

    // Define a timer variable
    clock_t ticks;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Selection Sort All Bids" << endl;
        cout << "  4. Quick Sort All Bids" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            bids = loadBids(csvPath);

            cout << bids.size() << " bids read" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 2:
            // Loop and display the bids read
            for (int i = 0; i < bids.size(); ++i) {
                displayBid(bids[i]);
            }
            cout << endl;

            break;

        case 3:
            // Initialize a timer variable before starting selection sort
            ticks = clock();

            // Call the selectionSort function
            selectionSort(bids);

            // Calculate elapsed time and display result
            ticks = clock() - ticks;
            cout << "Selection Sort completed in " << ticks << " clock ticks" << endl;
            cout << "Time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 4:
            // Initialize a timer variable before starting quick sort
            ticks = clock();

            // Call the quickSort function (from 0 to bids.size() - 1)
            quickSort(bids, 0, bids.size() - 1);

            // Calculate elapsed time and display result
            ticks = clock() - ticks;
            cout << "Quick Sort completed in " << ticks << " clock ticks" << endl;
            cout << "Time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 9:
            cout << "Good bye." << endl;
            break;

        default:
            cout << "Invalid option." << endl;
            break;
        }
    }

    return 0;
}
