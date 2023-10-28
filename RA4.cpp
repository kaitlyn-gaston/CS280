#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

std::vector<std::string> prepositions = {"about", "after", "below", "for", "from", "in", "into", "under", "with"};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "NO SPECIFIED INPUT FILE NAME." << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::ifstream inputFile(filename);

    if (!inputFile) {
        std::cout << "CANNOT OPEN THE FILE " << filename << std::endl;
        return 1;
    }

    std::map<std::string, int> prepositionCount;
    int lineCount = 0;
    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string word;
        lineCount++;

        while (iss >> word) {
            // Remove punctuation and convert to lowercase
            //word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);

            // Check if the word is a preposition
            if (std::find(prepositions.begin(), prepositions.end(), word) != prepositions.end()) {
                prepositionCount[word]++;
            }
        }
    }

    inputFile.close();
    
    if (lineCount == 0) {
        std::cout << "File is empty." << std::endl;
        return 1;
    }
    
    if (prepositionCount.empty()) {
        std::cout << "No Preposition words from the given list are found in the file." << std::endl;
        return 1;
    }
    
    std::cout << "List of Prepositions seen in the file and their number of occurrences:" << "\n\n";

    // Print prepositions and their counts
    for (const std::string& preposition : prepositions) {
        if (prepositionCount.find(preposition) != prepositionCount.end()) {
            std::cout << preposition << ": " << prepositionCount[preposition] << std::endl;
        }
    }

    // Find and print the most frequently occurring preposition
    std::string mostFrequentPreposition = prepositions[0];
    int maxCount = prepositionCount[mostFrequentPreposition];

    for (const std::string& preposition : prepositions) {
        if (prepositionCount[preposition] > maxCount) {
            mostFrequentPreposition = preposition;
            maxCount = prepositionCount[preposition];
        }
    }

    std::cout << "The preposition with maximum occurrences is \"" << mostFrequentPreposition << "\", which is found " << maxCount << " times." << std::endl;

    return 0;
}