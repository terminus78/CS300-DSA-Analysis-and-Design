//============================================================================
// Name        : CourseAdvising.cpp
// Author      : Shawn Way
//============================================================================

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// Define a structure to hold course information
struct Course {
    string courseId; // unique identifier
    string name;
    vector<string> prereqs;
    Course(string iD = "NoID", string courseName = "NoName", vector<string> prerequisites = {}) {
        courseId = iD;
        name = courseName;
        prereqs = prerequisites;
    }
};

// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // Default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // Initialize with a course
    Node(Course aCourse) :
            Node() {
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void recursiveDelete(Node* currentNode);
    void printCourse(Course& course);
    void recursivePrint(Node* currentNode);
    void recursiveRemoveList(Node* currentNode, vector<string>& buildList);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void DisplayCourses();
    void Insert(Course course);
    void Remove(string courseId);
    Course Search(string courseId);
    void Clean();
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // Initialize root
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // Recurse from root deleting every node
    this->recursiveDelete(root);
}

/**
* Recurse through tree and destroy all nodes
*/
void BinarySearchTree::recursiveDelete(Node* currentNode) {
    if (currentNode == nullptr) {
        return;
    }

    recursiveDelete(currentNode->left);
    recursiveDelete(currentNode->right);
    
    delete currentNode;
}

/**
* Display a course in an easily readable format
*/
void BinarySearchTree::printCourse(Course& course) {
    cout << course.courseId << ", " << course.name << endl;
}

/**
 * Traverse the tree in order and print courses
 */
void BinarySearchTree::recursivePrint(Node* currentNode) {
    if (currentNode == nullptr) {
        return;
    }

    this->recursivePrint(currentNode->left);

    this->printCourse(currentNode->course);

    this->recursivePrint(currentNode->right);
}

/**
 * Traverse the tree in order and build a list of courses to remove
 */
void BinarySearchTree::recursiveRemoveList(Node* currentNode, vector<string>& buildList) {
    if (currentNode == nullptr) {
        return;
    }

    this->recursiveRemoveList(currentNode->left, buildList);

    Course course = currentNode->course;

    for (int i = 0; i < course.prereqs.size(); i++) {
        if (((this->Search(course.courseId)).courseId == "NoID") && ((find(buildList.begin(), buildList.end(), course.courseId)) == buildList.end())) {
            buildList.push_back(course.courseId);
        }
    }

    this->recursiveRemoveList(currentNode->right, buildList);
}

/**
 * Display all courses in ascending order
 */
void BinarySearchTree::DisplayCourses() {
    this->recursivePrint(root);
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    Node* currentNode;
    
    // New node is the root
    if (root == nullptr) {
        root = new Node(course);
    }
    // Otherwise, find new node position
    else {
        currentNode = root;

        // Loop until position found
        while (currentNode != nullptr) {
            // Course ID must go somewhere towards the left
            if (currentNode->course.courseId > course.courseId) {
                // Insert node to the left
                if (currentNode->left == nullptr) {
                    currentNode->left = new Node(course);
                    currentNode = nullptr;
                }
                // Continue looking to the left
                else {
                    currentNode = currentNode->left;
                }
            }
            // Course ID must go somewhere towards the right
            else {
                // Insert node to the right
                if (currentNode->right == nullptr) {
                    currentNode->right = new Node(course);
                    currentNode = nullptr;
                }
                // Continue looking to the right
                else {
                    currentNode = currentNode->right;
                }
            }
        }
    }
}

/**
 * Remove a course
 */
void BinarySearchTree::Remove(string courseId) {
    Node* parentNode = nullptr;
    Node* currentNode = root;
    Node* successorNode = nullptr;
    Course tempCourse;

    // Loop while current node is valid
    while (currentNode != nullptr) {
        // Node found
        if (currentNode->course.courseId == courseId) {
            // Remove leaf node
            if (currentNode->left == nullptr && currentNode->right == nullptr) {
                // Remove root node
                if (parentNode == nullptr) {
                    root = nullptr;
                }
                // Remove left leaf
                else if (parentNode->left == currentNode) {
                    parentNode->left = nullptr;
                }
                // Remove right leaf
                else {
                    parentNode->right = nullptr;
                }
            }
            // Remove left node with one child
            else if (currentNode->right == nullptr) {
                // Remove root node
                if (parentNode == nullptr) {
                    root = currentNode->left;
                }
                // Remove left internal node
                else if (parentNode->left == currentNode) {
                    parentNode->left = currentNode->left;
                }
                // Remove right internal node
                else {
                    parentNode->right = currentNode->left;
                }
            }
            // Remove right node with one child
            else if (currentNode->left == nullptr) {
                // Remove root node
                if (parentNode == nullptr) {
                    root = currentNode->right;
                }
                // Remove left internal node
                else if (parentNode->left == currentNode) {
                    parentNode->left = currentNode->right;
                }
                // Remove right internal node
                else {
                    parentNode->right = currentNode->right;
                }
            }
            // Remove a node with two children
            else {
                // Initialize successor with right side
                successorNode = currentNode->right;

                // Loop through and set successor to left nodes until leftmost found
                while (successorNode->left != nullptr) {
                    successorNode = successorNode->left;
                }

                tempCourse = successorNode->course;  // Store successor data

                this->Remove(successorNode->course.courseId);  // Remove successor node

                currentNode->course = tempCourse;  // Update current node's data
            }
            // Node found, search complete
            return;
        }
        // Move the search to the left node
        else if (currentNode->course.courseId > courseId) {
            parentNode = currentNode;
            currentNode = currentNode->left;
        }
        // Move the search to the right node
        else {
            parentNode = currentNode;
            currentNode = currentNode->right;
        }
    }
    return;  // Node never found
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseId) {
    Course course;
    Node* currentNode = root;

    // Loop while current node is valid
    while (currentNode != nullptr) {
        // Node found
        if (currentNode->course.courseId == courseId) {
            course = currentNode->course;
            currentNode = nullptr;
        }
        // Search for node on the left
        else if (currentNode->course.courseId > courseId) {
            currentNode = currentNode->left;
        }
        // Search for node on the right
        else {
            currentNode = currentNode->right;
        }
    }

    return course;  // Return course, even if node not found
}

/**
* Clean out courses with prerequisites that don't exist
*/
void BinarySearchTree::Clean() {
    vector<string> removeList;
    this->recursiveRemoveList(root, removeList);

    for (int i = 0; i < removeList.size(); i++) {
        this->Remove(removeList.at(i));
    }
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Load a CSV file containing courses into a container
 */
bool loadBids(string csvPath, BinarySearchTree* bst) {
    ifstream inputStream;
    string currentLine;
    string currentItem;
    bool moreThan2;
    Course course;

    cout << "Loading CSV file " << csvPath << endl;

    // Open CSV file
    inputStream.open("Course_Data.csv");
    if (!inputStream.is_open()) {
        cout << "Failed to open file." << endl;
        return false;
    }

    // Loop while input can be gathered
    while (!inputStream.fail()) {
        moreThan2 = false;
        getline(inputStream, currentLine);

        // Check if there are 2 or more items on a line
        for (int i = 0; i < currentLine.size(); i++) {
            if (currentLine[i]) {
                moreThan2 = true;
                break;
            }
        }

        // Skip line if less than 2 items
        if (!moreThan2) {
            continue;
        }

        // Read in course ID and name
        stringstream stringReader(currentLine);
        getline(stringReader, course.courseId, ',');
        getline(stringReader, course.name, ',');

        // Clear out temp course prereq vector in prep for loading
        if (course.prereqs.size() > 0) {
            course.prereqs.clear();
        }

        // Load all prereqs into course
        while (getline(stringReader, currentItem, ',')) {
            course.prereqs.push_back(currentItem);
        }

        bst->Insert(course);  // Store course in binary search tree
    }

    // Not all data was gathered
    if (!inputStream.eof()) {
        cout << "Input failure." << endl;
        inputStream.close();
        return false;
    }

    // Close file and report success
    inputStream.close();
    return true;
}

/**
* Get verified course input string
*/
void getVerifiedInput(string askForInput, string& userCourseID) {
    bool badInput = true;

    // Loop until good input found
    while (badInput) {
        badInput = false;

        // Get input
        while (cout << askForInput && !(cin >> userCourseID)) {
            cin.clear();  // Reset fail flag
            cin.ignore(1000, '\n');  // Discard input

            cout << "Invalid input. Please ensure input is one word string." << endl;
        }

        // All courses are 7 characters long
        if (userCourseID.size() != 7) {
            cout << "Invalid input. Please ensure input looks similar to 'CSCI100'.";
            continue;
        }

        // Continue looking for input if it contains spaces
        for (int i = 0; i < userCourseID.size(); i++) {
            if (isspace(userCourseID[i])) {
                cout << "Invalid input. Please input only one word.";
                badInput = true;
                break;
            }
        }

        transform(userCourseID.begin(), userCourseID.end(), userCourseID.begin(), ::toupper);
    }
}

/**
* Display data for one course
*/
void displayCourse(Course course) {
    cout << course.courseId << ", " << course.name << endl;
    cout << "Prerequisites:" << endl;

    if (course.prereqs.size() < 1) {
        cout << "    No prerequisites" << endl;
    }

    for (int i = 0; i < course.prereqs.size(); i++) {
        cout << "    " << course.prereqs.at(i) << endl;
    }
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {
    // Define a binary search tree to hold all courses
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;
    string userCourseID;

    cout << "Welcome to the course planner." << endl;

    // User menu and choice handling
    int choice = 0;
    // Loop while exit is not chosen
    while (choice != 9) {
        cout << endl;
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display Course List" << endl;
        cout << "  3. Print Course Details" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        cout << endl;

        // Choice handling
        switch (choice) {

            // Load data structure
        case 1:
            loadBids("Course_Data.csv", bst);
            bst->Clean();
            break;

            // Display course list
        case 2:
            bst->DisplayCourses();
            break;

            // Print course details
        case 3:
            getVerifiedInput("Enter the course ID to see more information: ", userCourseID);
            course = bst->Search(userCourseID);

            if (course.courseId == "NoID") {
                cout << "Course does not exist. Please try again." << endl;
            }
            else {
                displayCourse(course);
            }
            break;
        }
    }

    cout << "Thank you for using the course planner." << endl;

    return 0;
}
