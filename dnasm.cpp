// This program runs DNAsm code from the DNAsm.bin file. 
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
    // for (int i = 0; i < length; i++) {
    //     cout << bitset<8>(codons[i]) << "/";
    // }
    // cout << endl;
    // determine purpose of each codon
    map<char, string> codon_map;
    codon_map[0b000000]

}