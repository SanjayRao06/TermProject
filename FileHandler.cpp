#include "FileHandler.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

// Constructor
FileHandler::FileHandler(AVLTree& tree) : tree(tree) {}

// Helper method to split a string by delimiter
std::vector<std::string> FileHandler::splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(trim(token));
    }
    
    return tokens;
}

// Helper method to trim whitespace from a string
std::string FileHandler::trim(const std::string& str) {
    auto start = std::find_if_not(str.begin(), str.end(), [](unsigned char c) {
        return std::isspace(c);
    });
    
    auto end = std::find_if_not(str.rbegin(), str.rend(), [](unsigned char c) {
        return std::isspace(c);
    }).base();
    
    return (start < end) ? std::string(start, end) : std::string();
}

// Process the input file
bool FileHandler::processFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }
    
    std::vector<std::string> lines;
    std::string line;
    
    // Read all lines from the file
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    
    file.close();
    
    // Process the lines
    int lineIndex = 0;
    while (lineIndex < lines.size()) {
        line = trim(lines[lineIndex]);
        
        // Skip empty lines
        if (line.empty()) {
            lineIndex++;
            continue;
        }
        
        // Check if the line is a command
        if (line[0] == '#') {
            int operation = 0;
            try {
                operation = std::stoi(line.substr(1));
            } catch (const std::exception& e) {
                std::cerr << "Error parsing operation: " << line << std::endl;
                lineIndex++;
                continue;
            }
            
            lineIndex++;
            
            // Handle different operations
            switch (operation) {
                case 1: // Insert student
                    handleInsertStudent(lines, lineIndex);
                    break;
                    
                case 2: // Modify student CGPA
                    if (lineIndex < lines.size()) {
                        handleModifyCGPA(lines[lineIndex]);
                        lineIndex++;
                    }
                    break;
                    
                case 3: // Add new course
                    if (lineIndex < lines.size()) {
                        handleAddCourse(lines[lineIndex]);
                        lineIndex++;
                    }
                    break;
                    
                case 4: // Modify course marks
                    if (lineIndex < lines.size()) {
                        handleModifyCourseMarks(lines[lineIndex]);
                        lineIndex++;
                    }
                    break;
                    
                case 5: // Delete course
                    if (lineIndex < lines.size()) {
                        handleDeleteCourse(lines[lineIndex]);
                        lineIndex++;
                    }
                    break;
                    
                case 6: // Delete student
                    if (lineIndex < lines.size()) {
                        handleDeleteStudent(lines[lineIndex]);
                        lineIndex++;
                    }
                    break;
                    
                case 7: // Print students with roll number greater than given value
                    if (tree.getIsRollNoKey() && lineIndex < lines.size()) {
                        handlePrintAboveRollNo(lines[lineIndex]);
                        lineIndex++;
                    } else {
                        lineIndex++;
                    }
                    break;
                    
                case 8: // Print students with roll number less than given value
                    if (tree.getIsRollNoKey() && lineIndex < lines.size()) {
                        handlePrintBelowRollNo(lines[lineIndex]);
                        lineIndex++;
                    } else {
                        lineIndex++;
                    }
                    break;
                    
                case 9: // Print students with roll number between two given values
                    if (tree.getIsRollNoKey() && lineIndex < lines.size()) {
                        handlePrintBetweenRollNo(lines[lineIndex]);
                        lineIndex++;
                    } else {
                        lineIndex++;
                    }
                    break;
                    
                case 10: // Print students with CGPA greater than given value
                    if (!tree.getIsRollNoKey() && lineIndex < lines.size()) {
                        handlePrintAboveCGPA(lines[lineIndex]);
                        lineIndex++;
                    } else {
                        lineIndex++;
                    }
                    break;
                    
                case 11: // Print students with CGPA less than given value
                    if (!tree.getIsRollNoKey() && lineIndex < lines.size()) {
                        handlePrintBelowCGPA(lines[lineIndex]);
                        lineIndex++;
                    } else {
                        lineIndex++;
                    }
                    break;
                    
                case 12: // Print students with CGPA between two given values
                    if (!tree.getIsRollNoKey() && lineIndex < lines.size()) {
                        handlePrintBetweenCGPA(lines[lineIndex]);
                        lineIndex++;
                    } else {
                        lineIndex++;
                    }
                    break;
                    
                default:
                    std::cerr << "Unknown operation: " << operation << std::endl;
                    lineIndex++;
                    break;
            }
        } else {
            lineIndex++;
        }
        
        // Display the tree after each operation
        tree.displayAll();
    }
    
    return true;
}

// Handle inserting a student
void FileHandler::handleInsertStudent(const std::vector<std::string>& lines, int& lineIndex) {
    if (lineIndex >= lines.size()) {
        return;
    }
    
    // Parse student data
    std::string studentLine = lines[lineIndex++];
    std::vector<std::string> studentData = splitString(studentLine, ',');
    
    if (studentData.size() < 4) {
        std::cerr << "Error: Invalid student data format" << std::endl;
        return;
    }
    
    int rollNo;
    std::string name;
    float cgpa;
    int numSubjects;
    
    try {
        rollNo = std::stoi(studentData[0]);
        name = studentData[1];
        cgpa = std::stof(studentData[2]);
        numSubjects = std::stoi(studentData[3]);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing student data: " << e.what() << std::endl;
        return;
    }
    
    // Parse course data
    std::vector<CourseInfo> courses;
    for (int i = 0; i < numSubjects && lineIndex < lines.size(); i++) {
        std::string courseLine = lines[lineIndex++];
        std::vector<std::string> courseData = splitString(courseLine, ',');
        
        if (courseData.size() < 2) {
            std::cerr << "Error: Invalid course data format" << std::endl;
            continue;
        }
        
        std::string courseCode = courseData[0];
        float marks;
        
        try {
            marks = std::stof(courseData[1]);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing course marks: " << e.what() << std::endl;
            continue;
        }
        
        courses.push_back(CourseInfo(courseCode, marks));
    }
    
    // Insert the student into the tree
    tree.insert(rollNo, name, cgpa, courses);
    std::cout << "Inserted student: " << rollNo << ", " << name << std::endl;
}

// Handle modifying a student's CGPA
void FileHandler::handleModifyCGPA(const std::string& line) {
    std::vector<std::string> data = splitString(line, ',');
    
    if (data.size() < 2) {
        std::cerr << "Error: Invalid CGPA modification format" << std::endl;
        return;
    }
    
    int rollNo;
    float newCGPA;
    
    try {
        rollNo = std::stoi(data[0]);
        newCGPA = std::stof(data[1]);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing CGPA modification data: " << e.what() << std::endl;
        return;
    }
    
    if (tree.updateCGPA(rollNo, newCGPA)) {
        std::cout << "Updated CGPA for student " << rollNo << " to " << newCGPA << std::endl;
    } else {
        std::cerr << "Error: Student " << rollNo << " not found" << std::endl;
    }
}

// Handle adding a course to a student
void FileHandler::handleAddCourse(const std::string& line) {
    std::vector<std::string> data = splitString(line, ',');
    
    if (data.size() < 3) {
        std::cerr << "Error: Invalid course addition format" << std::endl;
        return;
    }
    
    int rollNo;
    std::string courseCode;
    float marks;
    
    try {
        rollNo = std::stoi(data[0]);
        courseCode = data[1];
        marks = std::stof(data[2]);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing course addition data: " << e.what() << std::endl;
        return;
    }
    
    if (tree.addCourse(rollNo, courseCode, marks)) {
        std::cout << "Added course " << courseCode << " with marks " << marks << " to student " << rollNo << std::endl;
    } else {
        std::cerr << "Error: Could not add course to student " << rollNo << std::endl;
    }
}

// Handle modifying a course's marks
void FileHandler::handleModifyCourseMarks(const std::string& line) {
    std::vector<std::string> data = splitString(line, ',');
    
    if (data.size() < 3) {
        std::cerr << "Error: Invalid course marks modification format" << std::endl;
        return;
    }
    
    int rollNo;
    std::string courseCode;
    float newMarks;
    
    try {
        rollNo = std::stoi(data[0]);
        courseCode = data[1];
        newMarks = std::stof(data[2]);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing course marks modification data: " << e.what() << std::endl;
        return;
    }
    
    if (tree.updateCourseMarks(rollNo, courseCode, newMarks)) {
        std::cout << "Updated marks for course " << courseCode << " to " << newMarks << " for student " << rollNo << std::endl;
    } else {
        std::cerr << "Error: Could not update course marks for student " << rollNo << std::endl;
    }
}

// Handle deleting a course from a student
void FileHandler::handleDeleteCourse(const std::string& line) {
    std::vector<std::string> data = splitString(line, ',');
    
    if (data.size() < 2) {
        std::cerr << "Error: Invalid course deletion format" << std::endl;
        return;
    }
    
    int rollNo;
    std::string courseCode;
    
    try {
        rollNo = std::stoi(data[0]);
        courseCode = data[1];
    } catch (const std::exception& e) {
        std::cerr << "Error parsing course deletion data: " << e.what() << std::endl;
        return;
    }
    
    if (tree.removeCourse(rollNo, courseCode)) {
        std::cout << "Deleted course " << courseCode << " from student " << rollNo << std::endl;
    } else {
        std::cerr << "Error: Could not delete course from student " << rollNo << std::endl;
    }
}

// Handle deleting a student
void FileHandler::handleDeleteStudent(const std::string& line) {
    int rollNo;
    
    try {
        rollNo = std::stoi(trim(line));
    } catch (const std::exception& e) {
        std::cerr << "Error parsing student deletion data: " << e.what() << std::endl;
        return;
    }
    
    if (tree.remove(rollNo)) {
        std::cout << "Deleted student " << rollNo << std::endl;
    } else {
        std::cerr << "Error: Student " << rollNo << " not found" << std::endl;
    }
}

// Handle printing students with roll number greater than a given value
void FileHandler::handlePrintAboveRollNo(const std::string& line) {
    int rollNo;
    
    try {
        rollNo = std::stoi(trim(line));
    } catch (const std::exception& e) {
        std::cerr << "Error parsing roll number: " << e.what() << std::endl;
        return;
    }
    
    tree.displayStudentsAboveRollNo(rollNo);
}

// Handle printing students with roll number less than a given value
void FileHandler::handlePrintBelowRollNo(const std::string& line) {
    int rollNo;
    
    try {
        rollNo = std::stoi(trim(line));
    } catch (const std::exception& e) {
        std::cerr << "Error parsing roll number: " << e.what() << std::endl;
        return;
    }
    
    tree.displayStudentsBelowRollNo(rollNo);
}

// Handle printing students with roll number between two given values
void FileHandler::handlePrintBetweenRollNo(const std::string& line) {
    std::vector<std::string> data = splitString(line, ',');
    
    if (data.size() < 2) {
        std::cerr << "Error: Invalid roll number range format" << std::endl;
        return;
    }
    
    int minRoll, maxRoll;
    
    try {
        minRoll = std::stoi(data[0]);
        maxRoll = std::stoi(data[1]);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing roll number range: " << e.what() << std::endl;
        return;
    }
    
    tree.displayStudentsInRollNoRange(minRoll, maxRoll);
}

// Handle printing students with CGPA greater than a given value
void FileHandler::handlePrintAboveCGPA(const std::string& line) {
    float cgpa;
    
    try {
        cgpa = std::stof(trim(line));
    } catch (const std::exception& e) {
        std::cerr << "Error parsing CGPA: " << e.what() << std::endl;
        return;
    }
    
    tree.displayStudentsAboveCGPA(cgpa);
}

// Handle printing students with CGPA less than a given value
void FileHandler::handlePrintBelowCGPA(const std::string& line) {
    float cgpa;
    
    try {
        cgpa = std::stof(trim(line));
    } catch (const std::exception& e) {
        std::cerr << "Error parsing CGPA: " << e.what() << std::endl;
        return;
    }
    
    tree.displayStudentsBelowCGPA(cgpa);
}

// Handle printing students with CGPA between two given values
void FileHandler::handlePrintBetweenCGPA(const std::string& line) {
    std::vector<std::string> data = splitString(line, ',');
    
    if (data.size() < 2) {
        std::cerr << "Error: Invalid CGPA range format" << std::endl;
        return;
    }
    
    float minCGPA, maxCGPA;
    
    try {
        minCGPA = std::stof(data[0]);
        maxCGPA = std::stof(data[1]);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing CGPA range: " << e.what() << std::endl;
        return;
    }
    
    tree.displayStudentsInCGPARange(minCGPA, maxCGPA);
}