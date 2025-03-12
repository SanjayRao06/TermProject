#ifndef NODEOPERATIONS_H
#define NODEOPERATIONS_H

#include "node.h"

class NodeOperations
{
private:
    // Helper functions for balancing
    static int getHeight(Node *node);
    static int getBalanceFactor(Node *node);
    static void updateHeight(Node *node);

    // Rotation methods
    static Node *rotateRight(Node *y);
    static Node *rotateLeft(Node *x);

    // Insertion helper
    static Node *insertNode(Node *node, int rollNo, const std::string &name,
                            float cgpa, int numSubjects, CourseInfo *courses, bool isRollNoKey);

    // Comparison method based on key type
    static bool compareKeys(Node *node, int rollNo, float cgpa);

public:
    // Student Operations
    static Node *insertStudent(Node *&root, int rollNo, const std::string &name,float cgpa, int numSubjects, CourseInfo *courses, bool isRollNoKey);
    static bool deleteStudent(Node *&root, int rollNo);
    static bool modifyStudentCGPA(Node *&root, int rollNo, float newCGPA);

    // Course Operations
    static bool addCourse(Node *root, int rollNo, int courseId, float marks);
    static bool modifyCourse(Node *root, int rollNo, int courseId, float newMarks);
    static bool deleteCourse(Node *root, int rollNo, int courseId);

    // Display and Search Functions
    static void displayAll(Node *root);
    static Node *findStudent(Node *root, int rollNo);

    // Advanced Search Methods
    static void printStudentsAboveRollNo(Node *root, int rollNo);
    static void printStudentsBelowRollNo(Node *root, int rollNo);
    static void printStudentsBetweenRollNo(Node *root, int startRollNo, int endRollNo);
    static void printStudentsAboveCGPA(Node *root, float cgpa);
    static void printStudentsBelowCGPA(Node *root, float cgpa);
    static void printStudentsBetweenCGPA(Node *root, float startCGPA, float endCGPA);
};

#endif