#include "FileHandler.h"
#include "AVLTree.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        std::cout << "Usage: " << argv[0] << " <input_file> [key_type]" << std::endl;
        std::cout << "key_type: rollno (default) or cgpa" << std::endl;
        return 1;
    }
    
    // Determine key type
    bool isRollNoKey = true;
    if (argc == 3) {
        std::string keyType = argv[2];
        if (keyType == "cgpa") {
            isRollNoKey = false;
            std::cout << "Using CGPA as the key for the AVL tree." << std::endl;
        } else if (keyType == "rollno") {
            std::cout << "Using Roll Number as the key for the AVL tree." << std::endl;
        } else {
            std::cout << "Invalid key type. Use 'rollno' or 'cgpa'." << std::endl;
            return 1;
        }
    } else {
        std::cout << "Using Roll Number as the key for the AVL tree (default)." << std::endl;
    }
    
    // Create AVL tree with specified key type
    AVLTree tree(isRollNoKey);
    
    // Create file handler
    FileHandler fileHandler(tree);
    
    // Process input file
    std::string filename = argv[1];
    std::cout << "Processing file: " << filename << std::endl;
    
    if (!fileHandler.processFile(filename)) {
        std::cout << "Error processing file: " << filename << std::endl;
        return 1;
    }
    
    std::cout << "File processed successfully." << std::endl;
    return 0;
}
