#include "filehandler.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

vector<string> FileHandler::splitAndTrim(const string &str)
{
    vector<string> result;
    string current;
    for (char c : str)
    {
        if (c == ',')
        {
            result.push_back(trim(current));
            current.clear();
        }
        else if (!isspace(c))
        {
            current += c;
        }
    }

    if (!current.empty()) 
    {
        result.push_back(trim(current));
    }

    return result;
}

bool FileHandler::parseStudentData(const string &line, int &rollNo,
                                   string &name, float &cgpa, int &numSubjects)
{
    vector<string> parts = splitAndTrim(line);
    if (parts.size() < 4)
        return false;

    stringstream ss;
    ss << parts[0] << " " << parts[2] << " " << parts[3];
    ss >> rollNo >> cgpa >> numSubjects;
    name = parts[1];

    return !ss.fail();
}

bool FileHandler::parseCourseData(const string &line, int &courseId, float &marks)
{
    vector<string> parts = splitAndTrim(line);
    if (parts.size() < 2)
        return false;

    stringstream ss;
    ss << parts[0] << " " << parts[1];
    ss >> courseId >> marks;

    return !ss.fail();
}

bool FileHandler::processFile(const string &filename)
{
    ifstream file(filename);
    if (!file)
    {
        cout << "Error opening file: " << filename << endl;
        return false;
    }

    string line;
    vector<string> initialData;
    bool isInitialDatabase = false;

    while (getline(file, line))
    {
        if (line.empty())
            continue;
        
        if (line == "# 1")
        {
            isInitialDatabase = true;
            continue;
        }
        else if (line[0] == '#')
        {
            if (isInitialDatabase)
            {
                handleInitialList(initialData);
                initialData.clear();
            }
            isInitialDatabase = false;
            currentCommand = line;
            continue;
        }

        // Collect initial database entries
        if (isInitialDatabase)
        {
            initialData.push_back(line);
        }
        else
        {
            // Process other commands
            processLine(line);
        }
    }

    // Handle any remaining initial database entries
    if (isInitialDatabase && !initialData.empty())
    {
        handleInitialList(initialData);
    }

    file.close();
    return true;
}

void FileHandler::handleInitialList(const vector<string> &lines)
{
    int currentIndex = 0;
    while (currentIndex < lines.size())
    {
        int rollNo, numSubjects;
        float cgpa;
        string name;

        if (parseStudentData(lines[currentIndex], rollNo, name, cgpa, numSubjects))
        {
            CourseInfo *courses = nullptr; // Initialize to nullptr for safety

            if (numSubjects > 0) {
                courses = new CourseInfo[numSubjects];

                for (int i = 0; i < numSubjects && currentIndex + 1 + i < lines.size(); i++)
                {
                    int courseId;
                    float marks;
                    if (parseCourseData(lines[currentIndex + 1 + i], courseId, marks))
                    {
                        courses[i] = CourseInfo(courseId, marks);
                    }
                }
            }

            NodeOperations::insertStudent(root, rollNo, name, cgpa, numSubjects, courses, isRollNumberKey);
            
            delete[] courses; // Free memory for courses
            currentIndex += (numSubjects > 0) ? numSubjects + 1 : 1; // Move to next student
        }
        else
        {
            currentIndex++; // If parsing fails, just skip the line
        }
    }
    NodeOperations::displayAll(root);
}


void FileHandler::processLine(const string &line)
{
    cout << "Processing command: " << currentCommand << ", Line: " << line << endl;

    // Implement command processing similar to previous implementation
    // but using the new NodeOperations methods
    try {
        if (currentCommand == "# 2") {
            // Modify student CGPA
            int rollNo;
            float newCGPA;
            stringstream ss(line);
            ss >> rollNo >> newCGPA;
            NodeOperations::modifyStudentCGPA(root, rollNo, newCGPA);
            NodeOperations::displayAll(root);
        }
        // Implement other command handlers similarly...
    } catch (const exception& e) {
        cerr << "Error processing command: " << e.what() << endl;
    }
}

string FileHandler::trim(const string &str)
{
    int first = str.find_first_not_of(" \t\n");
    if (first == string::npos)
        return "";
    int last = str.find_last_not_of(" \t\n");
    return str.substr(first, (last - first + 1));
}
