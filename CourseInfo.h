#ifndef COURSEINFO_H
#define COURSEINFO_H

#include <string>

class CourseInfo {
private:
    std::string courseCode;  // Course code (e.g., "101")
    float marks;             // Marks obtained in the course

public:
    // Constructors
    CourseInfo();
    CourseInfo(const std::string& courseCode, float marks);
    
    // Getters
    std::string getCourseCode() const;
    float getMarks() const;
    
    // Setters
    void setCourseCode(const std::string& courseCode);
    void setMarks(float marks);
};

#endif