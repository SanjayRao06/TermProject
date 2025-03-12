#include "node_operations.h"
#include <iostream>
#include <algorithm>
using namespace std;

int NodeOperations::getHeight(Node* node) {
    return node ? node->getHeight() : 0;
}

int NodeOperations::getBalanceFactor(Node* node) {
    if (!node) return 0;
    return getHeight(node->getLeft()) - getHeight(node->getRight());
}
bool NodeOperations::deleteStudent(Node*& root, int rollNo) {
    if (!root) return false;

    // Find the node to delete
    if (rollNo < root->getRollNo()) {
        return deleteStudent(root->getLeft(), rollNo);
    } else if (rollNo > root->getRollNo()) {
        return deleteStudent(root->getRight(), rollNo);
    } else {
        // Node with only one child or no child
        if (!root->getLeft() || !root->getRight()) {
            Node* temp = root->getLeft() ? root->getLeft() : root->getRight();

            // No child case
            if (!temp) {
                temp = root;
                root = nullptr;
            } else { // One child case
                *root = *temp; // Copy the contents of the non-empty child
            }
            delete temp;
        } else {
            // Node with two children: Get the inorder successor (smallest in the right subtree)
            Node* temp = root->getRight();
            while (temp->getLeft()) {
                temp = temp->getLeft();
            }

            // Copy the inorder successor's data to this node
            root->setRollNo(temp->getRollNo());
            root->setName(temp->getName());
            root->setCGPA(temp->getCGPA());
            root->setNumSubjects(temp->getNumSubjects());
            root->setCourses(temp->getCourses());

            // Delete the inorder successor
            deleteStudent(root->getRight(), temp->getRollNo());
        }
    }

    // If the tree had only one node then return
    if (!root) return true;

    // Update height of the current node
    updateHeight(root);

    // Rebalance the tree
    int balance = getBalanceFactor(root);

    // Left Left Case
    if (balance > 1 && getBalanceFactor(root->getLeft()) >= 0) {
        root = rotateRight(root);
    }

    // Left Right Case
    if (balance > 1 && getBalanceFactor(root->getLeft()) < 0) {
        root->setLeft(rotateLeft(root->getLeft()));
        root = rotateRight(root);
    }

    // Right Right Case
    if (balance < -1 && getBalanceFactor(root->getRight()) <= 0) {
        root = rotateLeft(root);
    }

    // Right Left Case
    if (balance < -1 && getBalanceFactor(root->getRight()) > 0) {
        root->setRight(rotateRight(root->getRight()));
        root = rotateLeft(root);
    }

    return true;
}

bool NodeOperations::addCourse(Node* root, int rollNo, int courseId, float marks) {
    Node* student = findStudent(root, rollNo);
    if (!student) return false;

    CourseInfo* courses = student->getCourses();
    int numSubjects = student->getNumSubjects();

    // Check if the course already exists
    for (int i = 0; i < numSubjects; i++) {
        if (courses[i].getCourseId() == courseId) {
            return false; // Course already exists
        }
    }

    // Add the new course
    CourseInfo* newCourses = new CourseInfo[numSubjects + 1];
    for (int i = 0; i < numSubjects; i++) {
        newCourses[i] = courses[i];
    }
    newCourses[numSubjects] = CourseInfo(courseId, marks);

    student->setCourses(newCourses);
    student->setNumSubjects(numSubjects + 1);

    delete[] courses; // Free the old courses array
    return true;
}

bool NodeOperations::modifyCourse(Node* root, int rollNo, int courseId, float newMarks) {
    Node* student = findStudent(root, rollNo);
    if (!student) return false;

    CourseInfo* courses = student->getCourses();
    int numSubjects = student->getNumSubjects();

    for (int i = 0; i < numSubjects; i++) {
        if (courses[i].getCourseId() == courseId) {
            courses[i].setMarks(newMarks);
            return true;
        }
    }

    return false; // Course not found
}

bool NodeOperations::deleteCourse(Node* root, int rollNo, int courseId) {
    Node* student = findStudent(root, rollNo);
    if (!student) return false;

    CourseInfo* courses = student->getCourses();
    int numSubjects = student->getNumSubjects();

    // Find the course to delete
    int index = -1;
    for (int i = 0; i < numSubjects; i++) {
        if (courses[i].getCourseId() == courseId) {
            index = i;
            break;
        }
    }

    if (index == -1) return false; // Course not found

    // Shift courses to the left
    for (int i = index; i < numSubjects - 1; i++) {
        courses[i] = courses[i + 1];
    }

    student->setNumSubjects(numSubjects - 1);
    return true;
}

void NodeOperations::printStudentsAboveRollNo(Node* root, int rollNo) {
    if (!root) return;

    if (root->getRollNo() > rollNo) {
        printStudentsAboveRollNo(root->getLeft(), rollNo);
        cout << "Roll Number: " << root->getRollNo() << ", Name: " << root->getName() << ", CGPA: " << root->getCGPA() << endl;
    }
    printStudentsAboveRollNo(root->getRight(), rollNo);
}

void NodeOperations::printStudentsBelowRollNo(Node* root, int rollNo) {
    if (!root) return;

    printStudentsBelowRollNo(root->getLeft(), rollNo);
    if (root->getRollNo() < rollNo) {
        cout << "Roll Number: " << root->getRollNo() << ", Name: " << root->getName() << ", CGPA: " << root->getCGPA() << endl;
    }
    printStudentsBelowRollNo(root->getRight(), rollNo);
}

void NodeOperations::printStudentsBetweenRollNo(Node* root, int startRollNo, int endRollNo) {
    if (!root) return;

    if (root->getRollNo() > startRollNo) {
        printStudentsBetweenRollNo(root->getLeft(), startRollNo, endRollNo);
    }
    if (root->getRollNo() >= startRollNo && root->getRollNo() <= endRollNo) {
        cout << "Roll Number: " << root->getRollNo() << ", Name: " << root->getName() << ", CGPA: " << root->getCGPA() << endl;
    }
    if (root->getRollNo() < endRollNo) {
        printStudentsBetweenRollNo(root->getRight(), startRollNo, endRollNo);
    }
}

void NodeOperations::printStudentsAboveCGPA(Node* root, float cgpa) {
    if (!root) return;

    if (root->getCGPA() > cgpa) {
        printStudentsAboveCGPA(root->getLeft(), cgpa);
        cout << "Roll Number: " << root->getRollNo() << ", Name: " << root->getName() << ", CGPA: " << root->getCGPA() << endl;
    }
    printStudentsAboveCGPA(root->getRight(), cgpa);
}

void NodeOperations::printStudentsBelowCGPA(Node* root, float cgpa) {
    if (!root) return;

    printStudentsBelowCGPA(root->getLeft(), cgpa);
    if (root->getCGPA() < cgpa) {
        cout << "Roll Number: " << root->getRollNo() << ", Name: " << root->getName() << ", CGPA: " << root->getCGPA() << endl;
    }
    printStudentsBelowCGPA(root->getRight(), cgpa);
}

void NodeOperations::printStudentsBetweenCGPA(Node* root, float startCGPA, float endCGPA) {
    if (!root) return;

    if (root->getCGPA() > startCGPA) {
        printStudentsBetweenCGPA(root->getLeft(), startCGPA, endCGPA);
    }
    if (root->getCGPA() >= startCGPA && root->getCGPA() <= endCGPA) {
        cout << "Roll Number: " << root->getRollNo() << ", Name: " << root->getName() << ", CGPA: " << root->getCGPA() << endl;
    }
    if (root->getCGPA() < endCGPA) {
        printStudentsBetweenCGPA(root->getRight(), startCGPA, endCGPA);
    }
}
void NodeOperations::updateHeight(Node* node) {
    if (node) {
        node->setHeight(1 + max(getHeight(node->getLeft()), getHeight(node->getRight())));
    }
}

Node* NodeOperations::rotateRight(Node* y) {
    Node* x = y->getLeft();
    Node* T2 = x ? x->getRight() : nullptr;

    // Perform rotation
    if (x) x->setRight(y);
    y->setLeft(T2);

    // Update heights
    updateHeight(y);
    updateHeight(x);

    return x;
}

Node* NodeOperations::rotateLeft(Node* x) {
    Node* y = x->getRight();
    Node* T2 = y ? y->getLeft() : nullptr;

    // Perform rotation
    if (y) y->setLeft(x);
    x->setRight(T2);

    // Update heights
    updateHeight(x);
    updateHeight(y);

    return y;
}

bool NodeOperations::compareKeys(Node* node, int rollNo, float cgpa) {
    if (!node) return false;
    
    if (node->isRollNumberKey()) {
        return rollNo == node->getRollNo();
    } else {
        return cgpa == node->getCGPA();
    }
}

Node* NodeOperations::insertNode(Node* node, int rollNo, const std::string& name, 
                                 float cgpa, int numSubjects, CourseInfo* courses, bool isRollNoKey) {
    // Standard BST insertion
    if (!node) {
        return new Node(rollNo, name, cgpa, numSubjects, courses, isRollNoKey);
    }

    // Determine insertion path based on key type
    bool insertLeft;
    if (isRollNoKey) {
        insertLeft = (rollNo < node->getRollNo());
    } else {
        insertLeft = (cgpa < node->getCGPA());
    }

    // Recursive insertion
    if (insertLeft) {
        node->setLeft(insertNode(node->getLeft(), rollNo, name, cgpa, numSubjects, courses, isRollNoKey));
    } else {
        node->setRight(insertNode(node->getRight(), rollNo, name, cgpa, numSubjects, courses, isRollNoKey));
    }

    // Update height
    updateHeight(node);

    // Get balance factor
    int balance = getBalanceFactor(node);

    // Left Left Case
    if (balance > 1 && 
        ((isRollNoKey && rollNo < (node->getLeft()->getRollNo())) || 
         (!isRollNoKey && cgpa < node->getLeft()->getCGPA()))) {
        return rotateRight(node);
    }

    // Right Right Case
    if (balance < -1 && 
        ((isRollNoKey && rollNo > (node->getRight()->getRollNo())) || 
         (!isRollNoKey && cgpa > node->getRight()->getCGPA()))) {
        return rotateLeft(node);
    }

    // Left Right Case
    if (balance > 1 && 
        ((isRollNoKey && rollNo > (node->getLeft()->getRollNo())) || 
         (!isRollNoKey && cgpa > node->getLeft()->getCGPA()))) {
        node->setLeft(rotateLeft(node->getLeft()));
        return rotateRight(node);
    }

    // Right Left Case
    if (balance < -1 && 
        ((isRollNoKey && rollNo < (node->getRight()->getRollNo())) || 
         (!isRollNoKey && cgpa < node->getRight()->getCGPA()))) {
        node->setRight(rotateRight(node->getRight()));
        return rotateLeft(node);
    }

    return node;
}

Node* NodeOperations::insertStudent(Node*& root, int rollNo, const std::string& name, 
                                   float cgpa, int numSubjects, CourseInfo* courses, bool isRollNoKey) {
    root = insertNode(root, rollNo, name, cgpa, numSubjects, courses, isRollNoKey);
    return root;
}

Node* NodeOperations::findStudent(Node* root, int rollNo) {
    while (root) {
        if (rollNo == root->getRollNo()) return root;
        
        if (rollNo < root->getRollNo()) {
            root = root->getLeft();
        } else {
            root = root->getRight();
        }
    }
    return nullptr;
}

void NodeOperations::displayAll(Node* root) {
    if (!root) {
        cout << "Tree is empty." << endl;
        return;
    }

    // In-order traversal
    if (root->getLeft()) displayAll(root->getLeft());
    
    cout << "Roll Number: " << root->getRollNo() << endl;
    cout << "Name: " << root->getName() << endl;
    cout << "CGPA: " << root->getCGPA() << endl;
    cout << "Number of Subjects: " << root->getNumSubjects() << endl;
    
    cout << "\nCourses:" << endl;
    CourseInfo* courses = root->getCourses();
    for (int i = 0; i < root->getNumSubjects(); i++) {
        if (courses[i].getCourseId() != 0) {
            cout << "  Course ID: " << courses[i].getCourseId()
                 << ", Marks: " << courses[i].getMarks() << endl;
        }
    }
    cout << "-------------------" << endl;

    if (root->getRight()) displayAll(root->getRight());
}

bool NodeOperations::modifyStudentCGPA(Node*& root, int rollNo, float newCGPA) {
    Node* student = findStudent(root, rollNo);
    if (!student) return false;
    
    // Remove and reinsert to maintain tree balance
    int numSubjects = student->getNumSubjects();
    CourseInfo* courses = student->getCourses();
    std::string name = student->getName();
    
    // Delete the old node
    deleteStudent(root, rollNo);
    
    // Reinsert with new CGPA
    insertStudent(root, rollNo, name, newCGPA, numSubjects, courses, true);
    
    return true;
}

// Implement other methods similarly...
// Methods like addCourse, modifyCourse, deleteCourse, etc. 
// would follow a similar pattern of finding the node and modifying it