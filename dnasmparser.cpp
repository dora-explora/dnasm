// this file takes in text describing dnasm code and turns it into a file.

#include <iostream>
#include <fstream>
#include <sstream> 
#include <string>
#include <vector>
#include <bitset>

using namespace std;

class Token {
    public:
        string text; // text of the token
        char codon; // the tokens corresponding codon

    Token(string inputtext) {
        text = inputtext;
    }

    char bindecode() {
        char output = 0b000000;
        for (int i = 0; i < 6; i++) {
            if (text[i] == '1') {
                output |= (0b000001 << (5 - i));
            }
        }
        return output;
    } 

};



string open(string filename) {
    int length = 0;
    // read file
    ifstream file(filename, ios::binary | ios::in);
    // open file as array of bytes
    if (file.is_open()) {
        string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();
        return content;
    } else {
        // cout << "Error: Unable to open file" << endl;
        throw std::invalid_argument("Unable to open file; It either doesn't exist, or this program doesn't have the permissions.");
    }
}

int main() {
    string filename;
    string content;
    vector<Token> tokens; 
    // cout << "your filename: ";
    // cin >> filename;
    filename = "dnasmparserexample.txt";
    content = open(filename);
    istringstream stream(content);
    string workingtokentext; 
    while (getline(stream, workingtokentext, ' ') || getline(stream, workingtokentext, '\n')) {
        tokens.push_back(Token(workingtokentext));
    }
    for (Token workingtoken : tokens) {
        if (workingtoken.text[1] == '0' || workingtoken.text[1] == '1') {
            workingtoken.codon = workingtoken.bindecode(); 
        } else {
            switch(workingtoken.text) {

            }
        }
    }
    return 1;
}