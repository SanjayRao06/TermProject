#include "CourseInfo.h"
#include <iostream>

// Default constructor
CourseInfo::CourseInfo() : courseCode(""), marks(0.0) {}

// Parameterized constructor
CourseInfo::CourseInfo(const std::string& courseCode, float marks) 
    : courseCode(courseCode), marks(marks) {}

// Getters
std::string CourseInfo::getCourseCode() const {
    return courseCode;
}

float CourseInfo::getMarks() const {
    return marks;
}

// Setters
void CourseInfo::setCourseCode(const std::string& courseCode) {
    this->courseCode = courseCode;
}

void CourseInfo::setMarks(float marks) {
    this->marks = marks;
}