#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <string>


using namespace std;

bool inLanguage(string & inputString){

    // Split the input string into two parts: S and S'
    size_t pos = inputString.find("#");
    string S = inputString.substr(0, pos);
    string S_prime = inputString.substr(pos+1,inputString.length());
    
    // Check if the input string is empty
    if (inputString.empty()) {
        cout << "Empty string is not in the Language." << endl;
        return false;
    }else if (pos == string::npos) {
        cout << "string is missing #." << endl;
        return false;
    }

    // Create a queue and a stack
    queue<char> q;
    stack<char> s;

    // Push characters of S into the queue and the stack
    for (char c : S) {
        q.push(c);
    }
    
    for (char c : S_prime) {
        s.push(c);
    }

    // Compare characters from the queue and the stack to check if S' is the reverse of S
    while (!q.empty() && !s.empty()) {
        if (q.front() != s.top()) {
            return false;
        }
        q.pop();
        s.pop();
    }

    // If both the queue and stack are empty, and S' is the reverse of S, the input string is in the language
    return q.empty();
}
