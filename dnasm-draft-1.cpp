// This program runs DNAsm code from the DNAsm.bin file. 

#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>
#include <string>
#include <bitset>

using namespace std;

class Enzyme {
    public:
    vector<char> codons; // all the codons
    vector<string> decodons; // all the decodons
    int length; // length of enzymes code, starts at 0 (for iterating)
    int cursor; // cursor for where the enzyme is in the code
    int instrptr; // instruction pointer
    char subarg1; // first arg for substitution 
    char subarg2; // second arg for substitution 
    char insarg1; // first arg for insertion
    char insarg2; // second arg for insertion
    bool direction; // direction to travel in: true if forward, false if backward
    bool backslash; // for printing

    Enzyme(vector<char> incodons, vector<string> indecodons) {
        codons = incodons;
        decodons = indecodons;
        length = incodons.size() - 1;
        cursor = 0;
        instrptr = 0;
        direction = true;
        backslash = false;
    }

    void print(char rawinput) {
        char input = (rawinput | 0b0100000);
        if (backslash) {
            if (input == 0b01101110) {
                cout << endl; // for \n 
            }
        } else if (rawinput == 0b00111110) {
            cout <<  ' ';
        } else {
            cout << (0b01000000 || input);
        }
    }

    void step() {
        string current = decodons[instrptr];
        if (codons[instrptr] == subarg1) {
            codons[instrptr] = subarg2;
        } else if (current == "JmpCur") {
            cursor = 0;
            cursor |= (codons[instrptr + 1] & 0x3F) << 18;
            cursor |= (codons[instrptr + 2] & 0x3F) << 12;
            cursor |= (codons[instrptr + 3] & 0x3F) << 6;
            cursor |= (codons[instrptr + 4] & 0x3F);
            cout << bitset<24>(cursor) << endl;
            instrptr += 4;
        } else if (current == "JmpIns") {
            instrptr = 0;
            instrptr |= (codons[instrptr + 1] & 0x3F) << 18;
            instrptr |= (codons[instrptr + 2] & 0x3F) << 12;
            instrptr |= (codons[instrptr + 3] & 0x3F) << 6;
            instrptr |= (codons[instrptr + 4] & 0x3F);
            cout << bitset<24>(instrptr) << endl;
            instrptr += 4;
        } else if (current == "Substi") {
            subarg1 = codons[instrptr + 1];
            subarg2 = codons[instrptr + 2];
            instrptr += 2;
        } else if (current == "Advanc") {
            if (direction) {cursor++;} else {cursor--;}
        } else if (current == "SetFwd") {
            direction = true;
        } else if (current == "SetBwd") {
            direction = false;
        } else if (current == "Output") {
            print(codons[instrptr + 1]);
            instrptr += 1;
        } else if (current == "OutCur") {
            print(0b00111110);
        } else if (current == "Insert") {
            insarg1 = codons[instrptr + 1];
            insarg2 = codons[instrptr + 2];
            instrptr += 2;
        } else {
            instrptr++;
        }
    };
};

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
    // print file contents as binary (for debugging)
    // for (int i = 0; i < length; i++) {
    //     cout << bitset<8>(codons[i]) << "/";
    // }
    // cout << endl;

    // determine purpose of each codon in (codon, six character string) pairs
    bool attached = 0;
    bool writing = 0;
    string* decodons = new string[length];
    //  loop over codons and determine purpose
    for (int i = 0; i < length; i++) {
        if (codons[i] == 0b111110) {
            attached = 1;
            decodons[i] = "Attach";
        } else if (codons[i] == 0b110000 && attached) {
            writing = 1;
            decodons[i] = "BegPro";
            decodons[i+1] = "Marker";
            i++;
        } else if (codons[i] == 0b000011 && attached) {
            writing = 0;
            decodons[i] = "EndPro";
        } else if (codons[i] == 0b011111 && attached) {
            attached = 0;
            decodons[i] = "Detach";
        } else if (attached && !writing && codons[i] == 0b001100) {
            decodons[i] = "RunPro";
            decodons[i+1] = "Arg 1 ";
            decodons[i+2] = "Arg 2 ";
            decodons[i+3] = "Arg 3 ";
            decodons[i+4] = "Arg 4 ";
            i += 4;
        } else if (attached && !writing) {
            decodons[i] = "Ready ";
        } else if (attached && writing && codons[i] == 0b001111) {
            decodons[i] = "JmpCur";
            decodons[i+1] = "Arg 1 ";
            decodons[i+2] = "Arg 2 ";
            decodons[i+3] = "Arg 3 ";
            decodons[i+4] = "Arg 4 ";
            i += 4;
        } else if (attached && writing && codons[i] == 0b111100) {
            decodons[i] = "JmpIns";
        } else if (attached && writing && codons[i] == 0b110011) {
            decodons[i] = "Substi";
            decodons[i+1] = "Arg 1 ";
            decodons[i+2] = "Arg 2 ";
            i += 2;
        } else if (attached && writing && codons[i] == 0b111000) {
            decodons[i] = "Advanc";
        } else if (attached && writing && codons[i] == 0b100100) {
            decodons[i] = "SetFwd";
            decodons[i+1] = "Arg 1 ";
            i++;
        } else if (attached && writing && codons[i] == 0b011011) {
            decodons[i] = "SetBwd";
            decodons[i+1] = "Arg 1 ";
            i++;
        } else if (attached && writing && codons[i] == 0b000100) {
            decodons[i] = "Output";
            decodons[i+1] = "Arg 1 ";
            i++;
        } else if (attached && writing && codons[i] == 0b000101) {
            decodons[i] = "OutCur";
        } else if (attached && writing && codons[i] == 0b100101) {
            decodons[i] = "Insert";
            decodons[i+1] = "Arg 1 ";
            decodons[i+2] = "Arg 2 ";
            i += 2;
        } else if (attached && writing && codons[i] == 0b011110) {
            decodons[i] = "Execut";
        } else if (attached && writing && codons[i] == 0b000000) {
            decodons[i] = "Blank ";
        } else {
            decodons[i] = "      ";
        }
    }
    cout << "Codons/Decodons: " << endl;
    for (int i = 0; i < length; i++) {
        cout << "#" << i << ": 0b" << bitset<6>(codons[i]) << " - " << decodons[i] << endl;
    }
    char workingmarker;
    vector<char> workingcodons;
    vector<char> markers;
    vector<string> workingdecodons;
    map<char, Enzyme> enzymes;
    for (int i = 0; i < length; i++) {
        if (decodons[i] == "BegPro") {
            writing = 1;
            i++;
            markers.push_back(codons[i]);
            workingmarker = codons[i];
            i++;
            for (i = i; writing; i++) {
                if (decodons[i] == "EndPro") {
                    writing = false;
                } else {
                    workingcodons.push_back(codons[i]);
                    workingdecodons.push_back(decodons[i]);
                }
                // add all the codons to working codons/decodons and make an enzyme out of it
            }
            enzymes.insert(pair<char, Enzyme>(workingmarker,Enzyme (workingcodons, workingdecodons)));
            workingcodons.clear();
            workingdecodons.clear();
            i--;
        }
    }
    for (int i = 0; i < markers.size(); i++) {
        Enzyme currentenzyme = enzymes.at(markers[i]);
        cout << "\nEnzyme " << i << " with marker " << bitset<6>(markers[i]) << endl;
        for (int j = 0; j <= currentenzyme.length; j++) {
            cout << "0b" << bitset<6>(currentenzyme.codons[j]) << " - " << currentenzyme.decodons[j] << endl;
        }
    }
    vector<char> readiedmarkers;
    int workingcursor = 0;
    // loop for readying and running proteins
    for (int i = 0; i < length; i++) {
        if (decodons[i] == "Ready") {
            readiedmarkers.push_back(codons[i]);
        } else if (decodons[i] == "RunPro") {
            workingcursor |= (codons[i+1] & 0x3F) << 18;
            workingcursor |= (codons[i+2] & 0x3F) << 12;
            workingcursor |= (codons[i+3] & 0x3F) << 6;
            workingcursor |= (codons[i+4] & 0x3F);
            for (int j = 0; j < readiedmarkers.size(); j++) {
                enzymes.at(readiedmarkers[i]).cursor = workingcursor;
            }
            readiedmarkers.empty();
        }
    }

    delete[] codons;
    delete[] decodons;

    return 0;
}
