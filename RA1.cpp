#include <iostream>
#include <string>

using namespace std;

int main(){
    string first,last;
    int section;
    cout << "Please enter your first name >>\n";
    cin >> first;
    cout << "Please enter your last name >>\n";
    cin >> last;
    cout << "Please enter your CS 280 section number >>\n";
    cin >> section;
    cout << "Hello " << first << ", welcome to CS 280 section " << section << "\n";
    return 0;
}