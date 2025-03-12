#ifndef NODE_H
#define NODE_H

#include "CourseInfo.h"
#include <string>
#include <vector>

class Node {
private:
    int rollNo;                      // Student roll number
    std::string name;                // Student name
    float cgpa;                      // Student CGPA
    std::vector<CourseInfo> courses; // Courses enrolled
    
    Node* left;                      // Left child
    Node* right;                     // Right child
    int height;                      // Height of the node for AVL balancing
    
    bool isRollNoKey;                // Flag to indicate if roll number is the key (true) or CGPA is the key (false)

public:
    // Constructors
    Node();
    Node(int rollNo, const std::string& name, float cgpa, const std::vector<CourseInfo>& courses, bool isRollNoKey);
    
    // Getters
    int getRollNo() const;
    std::string getName() const;
    float getCGPA() const;
    std::vector<CourseInfo>& getCourses();
    const std::vector<CourseInfo>& getCourses() const;
    int getNumCourses() const;
    Node* getLeft() const;
    Node* getRight() const;
    int getHeight() const;
    bool getIsRollNoKey() const;
    
    // Setters
    void setRollNo(int rollNo);
    void setName(const std::string& name);
    void setCGPA(float cgpa);
    void setCourses(const std::vector<CourseInfo>& courses);
    void setLeft(Node* left);
    void setRight(Node* right);
    void setHeight(int height);
    
    // Course management methods
    bool addCourse(const std::string& courseCode, float marks);
    bool updateCourseMarks(const std::string& courseCode, float newMarks);
    bool removeCourse(const std::string& courseCode);
    CourseInfo* findCourse(const std::string& courseCode);
};

#endif // NODE_H