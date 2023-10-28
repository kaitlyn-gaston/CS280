#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
#include <stdexcept>

using namespace std;

int main() {
    string filename;
    cout << "Enter the name of a file to read from:";
    cin >> filename;
    cout << "\n\n";

    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        cerr << "File cannot be opened " << filename << endl;
        return 1;
    }

    int totalLines = 0;
    int commentedLines = 0;
    int formattedWords = 0;
    int boldCommands = 0;
    int italicCommands = 0;
    int regularCommands = 0;

    string line;
    int lineCount = 0;

    while (getline(inputFile, line)) {
        lineCount++;

        // Count total lines
        totalLines++;

        // Check for commented lines
        if (line.find("//") == 0) {
            commentedLines++;
            continue; // Skip commented lines
        }

        // Check for command lines
        if (line.find("##") == 0) {
            // Convert the command to lowercase for case-insensitive comparison
            string command = line.substr(2);
            
            if(command == "		   "){
                cout << "Error: Unrecognizable command in line " << lineCount;
                cout << "\n\n";
            }

            for (char &c : command) {
                c = tolower(c);
            }

            istringstream iss(command);
            ostringstream report_string;
            string word;
            

            while (iss >> word) {
                if (word == "bold") {
                    boldCommands++;
                    break;
                } else if (word == "italic") {
                    italicCommands++;
                    break;
                } else if (word == "regular") {
                    regularCommands++;
                    break;
                } else {
                    cout << "Error: Unrecognizable command in line " << lineCount;
                    cout << "\n\n";
                }
                break;
            }
        } else {
            // Count words in non-command lines
            istringstream iss(line);
            string word;
            while (iss >> word) {
                formattedWords++;
            }
        }
    }

    inputFile.close();
    
    if(totalLines == 0){
        cout << "File is Empty" << endl;
    }
    else{
        // Print the collected information
        cout << "Total lines: " << totalLines << endl;
        cout << "Commented lines: " << commentedLines << endl;
        cout << "Number of words in the Text to be formatted: " << formattedWords << endl;
        cout << "Bold commands: " << boldCommands << endl;
        cout << "Italic commands: " << italicCommands << endl;
        cout << "Regular commands: " << regularCommands << endl;
    }

    return 0;
}
