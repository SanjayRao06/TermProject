#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "AVLTree.h"
#include <string>
#include <vector>

class FileHandler {
private:
    AVLTree& tree;
    
    // Helper methods for parsing
    std::vector<std::string> splitString(const std::string& str, char delimiter);
    std::string trim(const std::string& str);
    
    // Operation handlers
    void handleInsertStudent(const std::vector<std::string>& lines, int& lineIndex);
    void handleModifyCGPA(const std::string& line);
    void handleAddCourse(const std::string& line);
    void handleModifyCourseMarks(const std::string& line);
    void handleDeleteCourse(const std::string& line);
    void handleDeleteStudent(const std::string& line);
    void handlePrintAboveRollNo(const std::string& line);
    void handlePrintBelowRollNo(const std::string& line);
    void handlePrintBetweenRollNo(const std::string& line);
    void handlePrintAboveCGPA(const std::string& line);
    void handlePrintBelowCGPA(const std::string& line);
    void handlePrintBetweenCGPA(const std::string& line);
    
public:
    // Constructor
    FileHandler(AVLTree& tree);
    
    // Process input file
    bool processFile(const std::string& filename);
};

#endif // FILEHANDLER_H