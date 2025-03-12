#include "AVLTree.h"
#include <algorithm>
#include <iostream>

// Constructor
AVLTree::AVLTree(bool isRollNoKey) : root(nullptr), isRollNoKey(isRollNoKey) {}

// Destructor
AVLTree::~AVLTree() {
    destroyTree(root);
}

// Helper method to clean up memory
void AVLTree::destroyTree(Node* node) {
    if (node != nullptr) {
        destroyTree(node->getLeft());
        destroyTree(node->getRight());
        delete node;
    }
}

// Get height of a node (nullptr nodes have height 0)
int AVLTree::getHeight(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return node->getHeight();
}

// Calculate balance factor of a node
int AVLTree::getBalanceFactor(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return getHeight(node->getLeft()) - getHeight(node->getRight());
}

// Right rotation
Node* AVLTree::rightRotate(Node* y) {
    Node* x = y->getLeft();
    Node* T2 = x->getRight();

    // Perform rotation
    x->setRight(y);
    y->setLeft(T2);

    // Update heights
    y->setHeight(std::max(getHeight(y->getLeft()), getHeight(y->getRight())) + 1);
    x->setHeight(std::max(getHeight(x->getLeft()), getHeight(x->getRight())) + 1);

    // Return new root
    return x;
}

// Left rotation
Node* AVLTree::leftRotate(Node* x) {
    Node* y = x->getRight();
    Node* T2 = y->getLeft();

    // Perform rotation
    y->setLeft(x);
    x->setRight(T2);

    // Update heights
    x->setHeight(std::max(getHeight(x->getLeft()), getHeight(x->getRight())) + 1);
    y->setHeight(std::max(getHeight(y->getLeft()), getHeight(y->getRight())) + 1);

    // Return new root
    return y;
}

// Insert a node into the AVL tree
Node* AVLTree::insertNode(Node* node, int rollNo, const std::string& name, float cgpa, const std::vector<CourseInfo>& courses) {
    // Perform standard BST insertion
    if (node == nullptr) {
        return new Node(rollNo, name, cgpa, courses, isRollNoKey);
    }

    // Determine which key to use for comparison
    if (isRollNoKey) {
        if (rollNo < node->getRollNo()) {
            node->setLeft(insertNode(node->getLeft(), rollNo, name, cgpa, courses));
        } else if (rollNo > node->getRollNo()) {
            node->setRight(insertNode(node->getRight(), rollNo, name, cgpa, courses));
        } else {
            // Duplicate roll numbers are not allowed
            return node;
        }
    } else {
        // Using CGPA as key
        if (cgpa < node->getCGPA()) {
            node->setLeft(insertNode(node->getLeft(), rollNo, name, cgpa, courses));
        } else if (cgpa > node->getCGPA()) {
            node->setRight(insertNode(node->getRight(), rollNo, name, cgpa, courses));
        } else {
            // For duplicate CGPAs, use roll number as secondary key
            if (rollNo < node->getRollNo()) {
                node->setLeft(insertNode(node->getLeft(), rollNo, name, cgpa, courses));
            } else if (rollNo > node->getRollNo()) {
                node->setRight(insertNode(node->getRight(), rollNo, name, cgpa, courses));
            } else {
                // Duplicate roll numbers are not allowed
                return node;
            }
        }
    }

    // Update height of current node
    node->setHeight(1 + std::max(getHeight(node->getLeft()), getHeight(node->getRight())));

    // Get balance factor to check if node became unbalanced
    int balance = getBalanceFactor(node);

    // Left Left Case
    if (balance > 1) {
        if (isRollNoKey) {
            if (rollNo < node->getLeft()->getRollNo()) {
                return rightRotate(node);
            }
            // Left Right Case
            else if (rollNo > node->getLeft()->getRollNo()) {
                node->setLeft(leftRotate(node->getLeft()));
                return rightRotate(node);
            }
        } else {
            if (cgpa < node->getLeft()->getCGPA()) {
                return rightRotate(node);
            }
            // Left Right Case
            else if (cgpa > node->getLeft()->getCGPA()) {
                node->setLeft(leftRotate(node->getLeft()));
                return rightRotate(node);
            }
        }
    }

    // Right Right Case
    if (balance < -1) {
        if (isRollNoKey) {
            if (rollNo > node->getRight()->getRollNo()) {
                return leftRotate(node);
            }
            // Right Left Case
            else if (rollNo < node->getRight()->getRollNo()) {
                node->setRight(rightRotate(node->getRight()));
                return leftRotate(node);
            }
        } else {
            if (cgpa > node->getRight()->getCGPA()) {
                return leftRotate(node);
            }
            // Right Left Case
            else if (cgpa < node->getRight()->getCGPA()) {
                node->setRight(rightRotate(node->getRight()));
                return leftRotate(node);
            }
        }
    }

    // Return unchanged node pointer
    return node;
}

// Find the node with minimum value in a tree
Node* AVLTree::findMinValueNode(Node* node) {
    Node* current = node;
    
    // Find the leftmost leaf
    while (current && current->getLeft() != nullptr) {
        current = current->getLeft();
    }
    
    return current;
}

// Delete a node from the AVL tree
Node* AVLTree::deleteNode(Node* root, int rollNo) {
    // Standard BST delete
    if (root == nullptr) {
        return root;
    }
    
    // Determine which key to use for comparison
    if (isRollNoKey) {
        if (rollNo < root->getRollNo()) {
            root->setLeft(deleteNode(root->getLeft(), rollNo));
        } else if (rollNo > root->getRollNo()) {
            root->setRight(deleteNode(root->getRight(), rollNo));
        } else {
            // Node to be deleted found
            
            // Node with only one child or no child
            if (root->getLeft() == nullptr || root->getRight() == nullptr) {
                Node* temp = root->getLeft() ? root->getLeft() : root->getRight();
                
                // No child case
                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                } else {
                    // One child case
                    *root = *temp; // Copy the contents of the non-empty child
                }
                
                delete temp;
            } else {
                // Node with two children
                Node* temp = findMinValueNode(root->getRight());
                
                // Copy the inorder successor's data to this node
                root->setRollNo(temp->getRollNo());
                root->setName(temp->getName());
                root->setCGPA(temp->getCGPA());
                root->setCourses(temp->getCourses());
                
                // Delete the inorder successor
                root->setRight(deleteNode(root->getRight(), temp->getRollNo()));
            }
        }
    } else {
        // Using CGPA as key, we need to find the node with the given roll number first
        Node* nodeToDelete = findNode(root, rollNo);
        if (nodeToDelete == nullptr) {
            return root; // Node not found
        }
        
        float cgpa = nodeToDelete->getCGPA();
        
        // Now delete the node based on CGPA and roll number
        if (cgpa < root->getCGPA()) {
            root->setLeft(deleteNode(root->getLeft(), rollNo));
        } else if (cgpa > root->getCGPA()) {
            root->setRight(deleteNode(root->getRight(), rollNo));
        } else {
            // CGPA matches, check roll number as secondary key
            if (rollNo < root->getRollNo()) {
                root->setLeft(deleteNode(root->getLeft(), rollNo));
            } else if (rollNo > root->getRollNo()) {
                root->setRight(deleteNode(root->getRight(), rollNo));
            } else {
                // Node to be deleted found
                
                // Node with only one child or no child
                if (root->getLeft() == nullptr || root->getRight() == nullptr) {
                    Node* temp = root->getLeft() ? root->getLeft() : root->getRight();
                    
                    // No child case
                    if (temp == nullptr) {
                        temp = root;
                        root = nullptr;
                    } else {
                        // One child case
                        *root = *temp; // Copy the contents of the non-empty child
                    }
                    
                    delete temp;
                } else {
                    // Node with two children
                    Node* temp = findMinValueNode(root->getRight());
                    
                    // Copy the inorder successor's data to this node
                    root->setRollNo(temp->getRollNo());
                    root->setName(temp->getName());
                    root->setCGPA(temp->getCGPA());
                    root->setCourses(temp->getCourses());
                    
                    // Delete the inorder successor
                    root->setRight(deleteNode(root->getRight(), temp->getRollNo()));
                }
            }
        }
    }
    
    // If the tree had only one node then return
    if (root == nullptr) {
        return root;
    }
    
    // Update height of the current node
    root->setHeight(1 + std::max(getHeight(root->getLeft()), getHeight(root->getRight())));
    
    // Get the balance factor to check if this node became unbalanced
    int balance = getBalanceFactor(root);
    
    // Left Left Case
    if (balance > 1 && getBalanceFactor(root->getLeft()) >= 0) {
        return rightRotate(root);
    }
    
    // Left Right Case
    if (balance > 1 && getBalanceFactor(root->getLeft()) < 0) {
        root->setLeft(leftRotate(root->getLeft()));
        return rightRotate(root);
    }
    
    // Right Right Case
    if (balance < -1 && getBalanceFactor(root->getRight()) <= 0) {
        return leftRotate(root);
    }
    
    // Right Left Case
    if (balance < -1 && getBalanceFactor(root->getRight()) > 0) {
        root->setRight(rightRotate(root->getRight()));
        return leftRotate(root);
    }
    
    return root;
}

// Find a node by roll number
Node* AVLTree::findNode(Node* root, int rollNo) {
    if (root == nullptr) {
        return nullptr;
    }
    
    if (rollNo == root->getRollNo()) {
        return root;
    }
    
    if (isRollNoKey) {
        if (rollNo < root->getRollNo()) {
            return findNode(root->getLeft(), rollNo);
        } else {
            return findNode(root->getRight(), rollNo);
        }
    } else {
        // If CGPA is the key, we need to search both subtrees
        Node* leftResult = findNode(root->getLeft(), rollNo);
        if (leftResult != nullptr) {
            return leftResult;
        }
        
        return findNode(root->getRight(), rollNo);
    }
}

// Find a node by CGPA
Node* AVLTree::findNodeByCGPA(Node* root, float cgpa) {
    if (root == nullptr) {
        return nullptr;
    }
    
    if (cgpa == root->getCGPA()) {
        return root;
    }
    
    if (!isRollNoKey) {
        if (cgpa < root->getCGPA()) {
            return findNodeByCGPA(root->getLeft(), cgpa);
        } else {
            return findNodeByCGPA(root->getRight(), cgpa);
        }
    } else {
        // If roll number is the key, we need to search both subtrees
        Node* leftResult = findNodeByCGPA(root->getLeft(), cgpa);
        if (leftResult != nullptr) {
            return leftResult;
        }
        
        return findNodeByCGPA(root->getRight(), cgpa);
    }
}

// Public method to insert a student
void AVLTree::insert(int rollNo, const std::string& name, float cgpa, const std::vector<CourseInfo>& courses) {
    root = insertNode(root, rollNo, name, cgpa, courses);
}

// Public method to remove a student
bool AVLTree::remove(int rollNo) {
    if (findNode(root, rollNo) == nullptr) {
        return false; // Student not found
    }
    
    root = deleteNode(root, rollNo);
    return true;
}

// Public method to search for a student by roll number
Node* AVLTree::search(int rollNo) {
    return findNode(root, rollNo);
}

// Public method to search for a student by CGPA
Node* AVLTree::searchByCGPA(float cgpa) {
    return findNodeByCGPA(root, cgpa);
}

// Update a student's CGPA
bool AVLTree::updateCGPA(int rollNo, float newCGPA) {
    Node* student = findNode(root, rollNo);
    if (student == nullptr) {
        return false; // Student not found
    }
    
    // If CGPA is the key, we need to remove and reinsert the node
    if (!isRollNoKey) {
        std::vector<CourseInfo> courses = student->getCourses();
        std::string name = student->getName();
        
        // Remove the student
        remove(rollNo);
        
        // Reinsert with new CGPA
        insert(rollNo, name, newCGPA, courses);
    } else {
        // If roll number is the key, we can simply update the CGPA
        student->setCGPA(newCGPA);
    }
    
    return true;
}

// Add a course to a student's record
bool AVLTree::addCourse(int rollNo, const std::string& courseCode, float marks) {
    Node* student = findNode(root, rollNo);
    if (student == nullptr) {
        return false; // Student not found
    }
    
    return student->addCourse(courseCode, marks);
}

// Update a course's marks
bool AVLTree::updateCourseMarks(int rollNo, const std::string& courseCode, float newMarks) {
    Node* student = findNode(root, rollNo);
    if (student == nullptr) {
        return false; // Student not found
    }
    
    return student->updateCourseMarks(courseCode, newMarks);
}

// Remove a course from a student's record
bool AVLTree::removeCourse(int rollNo, const std::string& courseCode) {
    Node* student = findNode(root, rollNo);
    if (student == nullptr) {
        return false; // Student not found
    }
    
    return student->removeCourse(courseCode);
}

// In-order traversal to display all students
void AVLTree::inOrderTraversal(Node* node) {
    if (node != nullptr) {
        inOrderTraversal(node->getLeft());
        
        // Display student information
        std::cout << "Roll No: " << node->getRollNo() << ", Name: " << node->getName()
                  << ", CGPA: " << node->getCGPA() << ", Courses: " << node->getNumCourses() << std::endl;
        
        // Display course information
        for (const auto& course : node->getCourses()) {
            std::cout << "  Course: " << course.getCourseCode() << ", Marks: " << course.getMarks() << std::endl;
        }
        
        inOrderTraversal(node->getRight());
    }
}

// Display all students
void AVLTree::displayAll() {
    std::cout << "Student Records:" << std::endl;
    inOrderTraversal(root);
    std::cout << std::endl;
}

// Print students with roll number in a specific range
void AVLTree::printStudentsInRollNoRange(Node* node, int minRoll, int maxRoll) {
    if (node == nullptr) {
        return;
    }
    
    // If current node's roll number is greater than minRoll, then check left subtree
    if (minRoll < node->getRollNo()) {
        printStudentsInRollNoRange(node->getLeft(), minRoll, maxRoll);
    }
    
    // If current node's roll number is in range, print it
    if (minRoll <= node->getRollNo() && node->getRollNo() <= maxRoll) {
        std::cout << "Roll No: " << node->getRollNo() << ", Name: " << node->getName()
                  << ", CGPA: " << node->getCGPA() << ", Courses: " << node->getNumCourses() << std::endl;
        
        // Display course information
        for (const auto& course : node->getCourses()) {
            std::cout << "  Course: " << course.getCourseCode() << ", Marks: " << course.getMarks() << std::endl;
        }
    }
    
    // If current node's roll number is less than maxRoll, then check right subtree
    if (maxRoll > node->getRollNo()) {
        printStudentsInRollNoRange(node->getRight(), minRoll, maxRoll);
    }
}

// Print students with CGPA in a specific range
void AVLTree::printStudentsInCGPARange(Node* node, float minCGPA, float maxCGPA) {
    if (node == nullptr) {
        return;
    }
    
    // Check left subtree
    printStudentsInCGPARange(node->getLeft(), minCGPA, maxCGPA);
    
    // If current node's CGPA is in range, print it
    if (minCGPA <= node->getCGPA() && node->getCGPA() <= maxCGPA) {
        std::cout << "Roll No: " << node->getRollNo() << ", Name: " << node->getName()
                  << ", CGPA: " << node->getCGPA() << ", Courses: " << node->getNumCourses() << std::endl;
        
        // Display course information
        for (const auto& course : node->getCourses()) {
            std::cout << "  Course: " << course.getCourseCode() << ", Marks: " << course.getMarks() << std::endl;
        }
    }
    
    // Check right subtree
    printStudentsInCGPARange(node->getRight(), minCGPA, maxCGPA);
}

// Print students with roll number greater than a specific value
void AVLTree::printStudentsAboveRollNo(Node* node, int rollNo) {
    if (node == nullptr) {
        return;
    }
    
    // If current node's roll number is greater than the given roll number, print it and check both subtrees
    if (node->getRollNo() > rollNo) {
        std::cout << "Roll No: " << node->getRollNo() << ", Name: " << node->getName()
                  << ", CGPA: " << node->getCGPA() << ", Courses: " << node->getNumCourses() << std::endl;
        
        // Display course information
        for (const auto& course : node->getCourses()) {
            std::cout << "  Course: " << course.getCourseCode() << ", Marks: " << course.getMarks() << std::endl;
        }
        
        printStudentsAboveRollNo(node->getLeft(), rollNo);
    }
    
    // Check right subtree
    printStudentsAboveRollNo(node->getRight(), rollNo);
}

// Print students with roll number less than a specific value
void AVLTree::printStudentsBelowRollNo(Node* node, int rollNo) {
    if (node == nullptr) {
        return;
    }
    
    // Check left subtree
    printStudentsBelowRollNo(node->getLeft(), rollNo);
    
    // If current node's roll number is less than the given roll number, print it
    if (node->getRollNo() < rollNo) {
        std::cout << "Roll No: " << node->getRollNo() << ", Name: " << node->getName()
                  << ", CGPA: " << node->getCGPA() << ", Courses: " << node->getNumCourses() << std::endl;
        
        // Display course information
        for (const auto& course : node->getCourses()) {
            std::cout << "  Course: " << course.getCourseCode() << ", Marks: " << course.getMarks() << std::endl;
        }
        
        printStudentsBelowRollNo(node->getRight(), rollNo);
    }
}

// Print students with CGPA greater than a specific value
void AVLTree::printStudentsAboveCGPA(Node* node, float cgpa) {
    if (node == nullptr) {
        return;
    }
    
    // Check left subtree
    printStudentsAboveCGPA(node->getLeft(), cgpa);
    
    // If current node's CGPA is greater than the given CGPA, print it
    if (node->getCGPA() > cgpa) {
        std::cout << "Roll No: " << node->getRollNo() << ", Name: " << node->getName()
                  << ", CGPA: " << node->getCGPA() << ", Courses: " << node->getNumCourses() << std::endl;
        
        // Display course information
        for (const auto& course : node->getCourses()) {
            std::cout << "  Course: " << course.getCourseCode() << ", Marks: " << course.getMarks() << std::endl;
        }
    }
    
    // Check right subtree
    printStudentsAboveCGPA(node->getRight(), cgpa);
}

// Print students with CGPA less than a specific value
void AVLTree::printStudentsBelowCGPA(Node* node, float cgpa) {
    if (node == nullptr) {
        return;
    }
    
    // Check left subtree
    printStudentsBelowCGPA(node->getLeft(), cgpa);
    
    // If current node's CGPA is less than the given CGPA, print it
    if (node->getCGPA() < cgpa) {
        std::cout << "Roll No: " << node->getRollNo() << ", Name: " << node->getName()
                  << ", CGPA: " << node->getCGPA() << ", Courses: " << node->getNumCourses() << std::endl;
        
        // Display course information
        for (const auto& course : node->getCourses()) {
            std::cout << "  Course: " << course.getCourseCode() << ", Marks: " << course.getMarks() << std::endl;
        }
    }
    
    // Check right subtree
    printStudentsBelowCGPA(node->getRight(), cgpa);
}

// Public methods for range queries
void AVLTree::displayStudentsInRollNoRange(int minRoll, int maxRoll) {
    std::cout << "Students with Roll No between " << minRoll << " and " << maxRoll << ":" << std::endl;
    printStudentsInRollNoRange(root, minRoll, maxRoll);
    std::cout << std::endl;
}

void AVLTree::displayStudentsInCGPARange(float minCGPA, float maxCGPA) {
    std::cout << "Students with CGPA between " << minCGPA << " and " << maxCGPA << ":" << std::endl;
    printStudentsInCGPARange(root, minCGPA, maxCGPA);
    std::cout << std::endl;
}

void AVLTree::displayStudentsAboveRollNo(int rollNo) {
    std::cout << "Students with Roll No greater than " << rollNo << ":" << std::endl;
    printStudentsAboveRollNo(root, rollNo);
    std::cout << std::endl;
}

void AVLTree::displayStudentsBelowRollNo(int rollNo) {
    std::cout << "Students with Roll No less than " << rollNo << ":" << std::endl;
    printStudentsBelowRollNo(root, rollNo);
    std::cout << std::endl;
}

void AVLTree::displayStudentsAboveCGPA(float cgpa) {
    std::cout << "Students with CGPA greater than " << cgpa << ":" << std::endl;
    printStudentsAboveCGPA(root, cgpa);
    std::cout << std::endl;
}

void AVLTree::displayStudentsBelowCGPA(float cgpa) {
    std::cout << "Students with CGPA less than " << cgpa << ":" << std::endl;
    printStudentsBelowCGPA(root, cgpa);
    std::cout << std::endl;
}

// Getter for root
Node* AVLTree::getRoot() const {
    return root;
}

// Getter for key type
bool AVLTree::getIsRollNoKey() const {
    return isRollNoKey;
} 