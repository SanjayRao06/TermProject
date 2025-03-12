#ifndef AVLTREE_H
#define AVLTREE_H

#include "Node.h"
#include <string>
#include <vector>

class AVLTree {
private:
    Node* root;
    bool isRollNoKey; // Flag to indicate if roll number is the key (true) or CGPA is the key (false)
    
    // Helper methods for AVL tree operations
    int getHeight(Node* node);
    int getBalanceFactor(Node* node);
    Node* rightRotate(Node* y);
    Node* leftRotate(Node* x);
    
    // Internal methods for tree operations
    Node* insertNode(Node* node, int rollNo, const std::string& name, float cgpa, const std::vector<CourseInfo>& courses);
    Node* deleteNode(Node* root, int rollNo);
    Node* findMinValueNode(Node* node);
    Node* findNode(Node* root, int rollNo);
    Node* findNodeByCGPA(Node* root, float cgpa);
    
    // Helper methods for range queries
    void inOrderTraversal(Node* node);
    void printStudentsInRollNoRange(Node* node, int minRoll, int maxRoll);
    void printStudentsInCGPARange(Node* node, float minCGPA, float maxCGPA);
    void printStudentsAboveRollNo(Node* node, int rollNo);
    void printStudentsBelowRollNo(Node* node, int rollNo);
    void printStudentsAboveCGPA(Node* node, float cgpa);
    void printStudentsBelowCGPA(Node* node, float cgpa);

public:
    // Constructor
    AVLTree(bool isRollNoKey = true);
    
    // Destructor
    ~AVLTree();
    
    // Tree operations
    void insert(int rollNo, const std::string& name, float cgpa, const std::vector<CourseInfo>& courses);
    bool remove(int rollNo);
    Node* search(int rollNo);
    Node* searchByCGPA(float cgpa);
    
    // Student record operations
    bool updateCGPA(int rollNo, float newCGPA);
    bool addCourse(int rollNo, const std::string& courseCode, float marks);
    bool updateCourseMarks(int rollNo, const std::string& courseCode, float newMarks);
    bool removeCourse(int rollNo, const std::string& courseCode);
    
    // Range query operations
    void displayAll();
    void displayStudentsInRollNoRange(int minRoll, int maxRoll);
    void displayStudentsInCGPARange(float minCGPA, float maxCGPA);
    void displayStudentsAboveRollNo(int rollNo);
    void displayStudentsBelowRollNo(int rollNo);
    void displayStudentsAboveCGPA(float cgpa);
    void displayStudentsBelowCGPA(float cgpa);
    
    // Helper method to clean up memory
    void destroyTree(Node* node);
    
    // Getter for root
    Node* getRoot() const;
    
    // Getter for key type
    bool getIsRollNoKey() const;
};

#endif // AVLTREE_H 