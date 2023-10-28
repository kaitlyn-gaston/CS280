#include <iostream>
#include <fstream>
#include <string>
#include <regex>

bool isValidSpecialName(const std::string& word) {
    // Define a regex pattern to match valid special names
    std::regex pattern("^[@_][a-zA-Z][a-zA-Z0-9_]*$");
    return std::regex_match(word, pattern);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "NO SPECIFIED INPUT FILE NAME." << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::ifstream inputFile(filename);

    if (!inputFile) {
        std::cout << "CANNOT OPEN THE FILE " << filename << std::endl;
        return 1;
    }

    std::string line;
    int wordCount = 0;
    int specialNameCount = 0;

    while (std::getline(inputFile, line)) {
        if (line.empty()) {
            continue; // Skip empty lines
        }

        // Tokenize the line by whitespace
        std::istringstream iss(line);
        std::string word;
        while (iss >> word) {
            if (isValidSpecialName(word)) {
                specialNameCount++;
            }
            wordCount++;
        }
    }

    if (wordCount == 0) {
        std::cout << "File is empty." << std::endl;
    } else if (argc == 2) {
        std::cout << "Total number of words: " << wordCount << std::endl;
    } else if (argc == 3) {
        std::string flag = argv[2];
        if (flag == "-all") {
            std::cout << "Total number of words: " << wordCount << std::endl;
            std::cout << "Total number of special names: " << specialNameCount << std::endl;
        } else if (flag == "-sp") {
            std::cout << "Total number of special names: " << specialNameCount << std::endl;
        } else {
            std::cout << "UNRECOGNIZED FLAG " << flag << std::endl;
            return 1;
        }
    }

    inputFile.close();
    return 0;
}
