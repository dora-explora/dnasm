/*
This is a program designed to open the file DNAsm.bin and execute the contents of the file as DNAsm code.
This is for a esoteric programming language called DeoxyriboNucleic Assembly (DNAsm).
The following text is from the official DNAsm documentation:
DNAsm
Language is built by appending a string of protein instructions to the beginning of a string of data, which it modifies, and interprets as instructions. Multiple proteins can run simultaneously, and can potentially move along unwanted strings of data if not careful.

Ribosomal Transcription Unit
RTU Instructions
000000 - Begin transcription, where RTU attaches to data string
001100 - End transcription program, where RTU attaches to data string
110000 - Begin enzyme
000011 - End enzyme
101101 - Begin and end comment (comments are ascii with a prefix of 0b01, so all ascii letters from 01000000 to 01111111, 64 to 127)

Enzyme Instructions
110011 - Substitution: instruction 110011ab replaces all a with b
         a and b are not executed, 110011/000000/001100 would not
         affect the RTU
111000 - Advance: Advances to next codon.
100100 - Set Forward: Sets the direction to forward. 100100a sets
         direction to forward when a occurs, steps afterward
011011 - Set Backward: Sets the direction to forward. 100100a sets
         direction to backward when a occurs, steps afterward
000100 - Output: 000100a will push a to the output strand
000101 - Output at Pointer: 000101 will output the value of the
         current location codon to the output strand.
100101 - Insert: 100101ab will insert b when a is seen.
011111 - Execute: 011111a sets a to execute command. a/b will add
         b to enzyme instruction.

This program displays the current code on one line and the instructions/arguments the codon represents
on a second line. The program will also write the code to a file called DNAsm.bin.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>

using namespace std;

int main() {
    // read file
    ifstream file("DNAsm.bin", ios::binary | ios::in);
    char* codons;
    int length;
    // open file as array of bytes
    if (file.is_open()) {
        file.seekg(0, file.end);
        length = file.tellg();
        file.seekg(0, file.beg);
        codons = new char[length];
        file.read(codons, length);
        file.close();
    } else {
        cout << "Error: Unable to open file" << endl;
    }
    // print file contents as binary
    for (int i = 0; i < length; i++) {
        cout << bitset<6>(codons[i]) << " ";
    }

}