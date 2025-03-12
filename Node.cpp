#include "Node.h"
#include <algorithm>

// Default constructor
Node::Node() : rollNo(0), name(""), cgpa(0.0), left(nullptr), right(nullptr), height(1), isRollNoKey(true) {}

// Parameterized constructor
Node::Node(int rollNo, const std::string& name, float cgpa, const std::vector<CourseInfo>& courses, bool isRollNoKey)
    : rollNo(rollNo), name(name), cgpa(cgpa), courses(courses), left(nullptr), right(nullptr), height(1), isRollNoKey(isRollNoKey) {}

// Getters
int Node::getRollNo() const {
    return rollNo;
}

std::string Node::getName() const {
    return name;
}

float Node::getCGPA() const {
    return cgpa;
}

std::vector<CourseInfo>& Node::getCourses() {
    return courses;
}

const std::vector<CourseInfo>& Node::getCourses() const {
    return courses;
}

int Node::getNumCourses() const {
    return courses.size();
}

Node* Node::getLeft() const {
    return left;
}

Node* Node::getRight() const {
    return right;
}

int Node::getHeight() const {
    return height;
}

bool Node::getIsRollNoKey() const {
    return isRollNoKey;
}

// Setters
void Node::setRollNo(int rollNo) {
    this->rollNo = rollNo;
}

void Node::setName(const std::string& name) {
    this->name = name;
}

void Node::setCGPA(float cgpa) {
    this->cgpa = cgpa;
}

void Node::setCourses(const std::vector<CourseInfo>& courses) {
    this->courses = courses;
}

void Node::setLeft(Node* left) {
    this->left = left;
}

void Node::setRight(Node* right) {
    this->right = right;
}

void Node::setHeight(int height) {
    this->height = height;
}

// Course management methods
bool Node::addCourse(const std::string& courseCode, float marks) {
    // Check if course already exists
    for (const auto& course : courses) {
        if (course.getCourseCode() == courseCode) {
            return false; // Course already exists
        }
    }
    
    // Add new course
    courses.push_back(CourseInfo(courseCode, marks));
    return true;
}

bool Node::updateCourseMarks(const std::string& courseCode, float newMarks) {
    for (auto& course : courses) {
        if (course.getCourseCode() == courseCode) {
            course.setMarks(newMarks);
            return true;
        }
    }
    return false; // Course not found
}

bool Node::removeCourse(const std::string& courseCode) {
    auto it = std::find_if(courses.begin(), courses.end(), 
                          [&courseCode](const CourseInfo& course) {
                              return course.getCourseCode() == courseCode;
                          });
    
    if (it != courses.end()) {
        courses.erase(it);
        return true;
    }
    return false; // Course not found
}

CourseInfo* Node::findCourse(const std::string& courseCode) {
    for (auto& course : courses) {
        if (course.getCourseCode() == courseCode) {
            return &course;
        }
    }
    return nullptr; // Course not found
}