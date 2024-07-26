// this file takes in text describing dnasm code and turns it into a file.

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Token {
    public:
        string text; // text of the token
        char codon; // the tokens corresponding codon

    Token(string intext) {
        text = intext;
    }

    char bindecode(string input) {
        char output = 0b000000;
        
        return output;
    } 

};



void open(string filename) {
    int length = 0;
    char* bytes;
    // read file
    ifstream file(filename, ios::binary | ios::in);
    // open file as array of bytes
    if (file.is_open()) {
        file.seekg(0, file.end);
        length = file.tellg();
        file.seekg(0, file.beg);
        bytes = new char[length];
        file.read(bytes, length);
        file.close();
    } else {
        cout << "Error: Unable to open file" << endl;
        // throw std::invalid_argument("Unable to open file; It either doesn't exist, or this program doesn't have permissions.");
    }
}

int main() {
    string filename;
    cout << "your filename: ";
    cin >> filename;
    open(filename);
    vector<Token> tokens;
    return 1;
}