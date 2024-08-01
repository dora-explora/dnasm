// this file takes in text describing dnasm code and turns it into a file.

#include <iostream>
#include <fstream>
#include <sstream> 
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <bitset>

using namespace std;

map<string, char> codontable {
    {"Attach", 0b111110},
    {"Detach", 0b011111},
    {"RibJmp", 0b011110},
    {"BegPro", 0b110000},
    {"EndPro", 0b000011},
    {"RunPro", 0b001100},
    {"Commnt", 0b101101},

    {"CurJmp", 0b001111},
    {"InsJmp", 0b111100},
    {"Substi", 0b110011},
    {"Replac", 0b100101},
    {"StpFwd", 0b111000},
    {"StpBwd", 0b000111},
    {"Output", 0b000100},
    {"OutCur", 0b000101},
    {"Execut", 0b111111},
    {"Blank ", 0b000000},
};

class Token {
    public:
        string text; // text of the token
        char codon; // the tokens corresponding codon

    Token(string inputtext) {
        text = inputtext;
        codon = 0b000000;
    }
    
    void decode() {
        codon = codontable.at(text);
    }

    void bindecode() {
        for (int i = 0; i < 6; i++) {
            if (text[i] == '1') {
                codon |= (0b000001 << (5 - i));
            }
        }
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
        throw invalid_argument("Unable to open file; It either doesn't exist, or this program doesn't have the permissions.");
    }
}

void write(string filename, vector<Token> tokens) {
    ofstream file(filename, ios::binary | ios::out);
    if (file.is_open()) {
        for (Token& token : tokens) {
            file.write(reinterpret_cast<const char*>(&token.codon), sizeof(char));
        }
        file.close();
        cout << "Codons written to " << filename << endl;
    } else {
        cerr << "Error: Unable to open file for writing" << endl;
    }
}

int main() {
    string filename;
    string content;
    vector<Token> tokens; 
    // cout << "Please enter the files name: ";
    // cin >> filename;
    filename = "dnasmparserexampleB.txt";
    content = open(filename);
    replace(content.begin(), content.end(), '\n', ' ');
    istringstream stream(content);
    string workingtokentext; 
    while (getline(stream, workingtokentext, ' ') || getline(stream, workingtokentext, '\n')) {
        if (workingtokentext[0] != ' ' && workingtokentext[0] != '\n' && workingtokentext != "") { 
            tokens.push_back(Token(workingtokentext)); 
        }
    }
    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i].text[1] == '0' || tokens[i].text[1] == '1') {
            tokens[i].bindecode(); 
        } else {
            tokens[i].decode();
        }
        cout << "0b" << bitset<6>(tokens[i].codon) << endl;
    }
    // cout << "Please enter the name of your output file: ";
    // cin >> filename;
    filename = "dnasmparserexampleB.bin";
    write(filename, tokens);
    return 1;
}