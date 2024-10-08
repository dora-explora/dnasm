// This program enables the user to write codons with dfjk on their keyboard. After they're done, this 
// displays the current code on one line and the instructions/arguments the codon represents on a 
// second line then writes the code to the file DNAsm.bin.

// WIP!!!

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    char input;
    string codons;
    bool keepgoing = true;
    while (keepgoing) {
        cin >> input;
        switch (input) {
            case 'd':
                codons += "00";
                cout << codons << endl;
                break;
            case 'f':
                codons += "01";
                cout << codons << endl;
                break;
            case 'j':
                codons += "10";
                cout << codons << endl;
                break;
            case 'k':
                codons += "11";
                cout << codons << endl;
                break;
            case 'x':
                cout << "final code: " << codons << endl;
                keepgoing = false;
                break;
            }
    }
    cout << "loop done!";
};