// This program runs DNAsm code from the DNAsm.bin file. 

#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>
#include <string>
#include <bitset>
#include <tuple>

#include "dnasm.h"

using namespace std;

string* decodons; // all decodons in strings (human readable and properly identified)
char* codons; // all codons in chars (single bytes)
uint32_t length; // length of file in bytes (codons)

// Enzyme definition
Enzyme::Enzyme(vector<char> incodons, vector<string> indecodons) {
    codons = incodons;
    decodons = indecodons;
    length = incodons.size() - 1;
    cursor = 0;
    instrptr = 0;
    cooldown = 0;
    backslash = false;
}

void Enzyme::print(char rawinput) {
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
void Enzyme::cursorjump () { 
    cursor = 0;
    cursor |= (codons[instrptr + 1] & 0x3F) << 18;
    cursor |= (codons[instrptr + 2] & 0x3F) << 12;
    cursor |= (codons[instrptr + 3] & 0x3F) << 6;
    cursor |= (codons[instrptr + 4] & 0x3F);
    // cout << "Cursor: " << bitset<24>(cursor) << endl;
}

void Enzyme::instructionjump () {
    int tempinstrptr = instrptr;
    instrptr = 0;
    instrptr |= (codons[tempinstrptr - 3] & 0x3F) << 18;
    instrptr |= (codons[tempinstrptr - 2] & 0x3F) << 12;
    instrptr |= (codons[tempinstrptr - 1] & 0x3F) << 6;
    instrptr |= (codons[tempinstrptr] & 0x3F);
    // cout << "Instruction pointer: " << bitset<24>(instrptr) << endl;
}

char* Enzyme::step(char* globalcodons) {

    string current = decodons[instrptr];
    if (globalcodons[cursor] == subarg1 && substituting) {
        globalcodons[cursor] = subarg2;
    }
    if (cooldown > 0) {
        if (cooldown <= 2 && decodons[(instrptr - 3 + cooldown)] == "Substi") {
            if (cooldown == 2) { 
                subarg1 = codons[instrptr]; 
                substituting = false;
            }
            if (cooldown == 1) {
                subarg2 = codons[instrptr];
                substituting = true;
            }
        } else if (cooldown == 1 && decodons[instrptr - 1] == "Replac") {   
            globalcodons[instrptr - 1] = codons[instrptr];
        } else if (cooldown == 1 && decodons[instrptr - 4] == "CurJmp") { 
            cursorjump(); 
        } else if (cooldown == 1 && decodons[instrptr - 4] == "InsJmp") { 
            instructionjump();
            instrptr--; // could have adverse effects... 
        }
        cooldown--;
    } else if (current == "CurJmp") {
        cooldown = 4;
    } else if (current == "InsJmp") {   
        cooldown = 4;
    } else if (current == "Substi") {
        cooldown = 2;
    } else if (current == "Replac") {
        cooldown = 1;
    } else if (current == "StpFwd") {
        cursor++;
    } else if (current == "StpBwd") {
        cursor--;
    } else if (current == "Output") {
        print(codons[instrptr + 1]);
    }
    instrptr++;
    return globalcodons;
};

map<char, Enzyme> enzymes; // all enzymes and their markers
vector<char> markers; // all markers in order of when their enzymes were written, readied, running, or not.

// Ribosome definition
void DNAsm::runproteins() {
    for (uint32_t i = 0; i < readiedmarkers.size(); i++) { 
        runningmarkers.push_back(readiedmarkers[i]); 
        initialcursor = 0;
        initialcursor |= (codons[ribcursor-3] & 0x3F) << 18;
        initialcursor |= (codons[ribcursor-2] & 0x3F) << 12;
        initialcursor |= (codons[ribcursor-1] & 0x3F) << 6;
        initialcursor |= (codons[ribcursor] & 0x3F);
        enzymes.at(readiedmarkers[i]).cursor = initialcursor;
    }
    readiedmarkers.clear();
}

void DNAsm::ribosomejump() {
    int tempribcursor = ribcursor;
    ribcursor = 0;
    ribcursor |= (codons[tempribcursor-3] & 0x3F) << 18;
    ribcursor |= (codons[tempribcursor-2] & 0x3F) << 12;
    ribcursor |= (codons[tempribcursor-2] & 0x3F) << 6;
    ribcursor |= (codons[tempribcursor] & 0x3F);
}

void DNAsm::ribstep() {
    ribcurrent = decodons[ribcursor];
    if (commenting) {
        // do absolutely nothing, its a comment
        if (ribcurrent == "Commnt") { commenting = false; }
    }
    else if (cooldown != 0) {
        if (cooldown == 1) {
            if (decodons[ribcursor - 4] == "RunPro") {
                runproteins();
            } else if (decodons[ribcursor - 4] == "RibJmp") {
                ribosomejump();
            }
        }
        cooldown--;
    } else if (!attached && !writing && ribcurrent == "Attach") {
        attached = true;
    } else if (attached && !writing && ribcurrent == "Detach") {
        attached = false;
    } else if (attached && writing && ribcurrent == "EndPro") {
        enzymes.insert(pair<char, Enzyme>(workingmarker,Enzyme (workingcodons, workingdecodons)));
        markers.push_back(workingmarker);
        workingmarker = 0b00000000;
        workingcodons.clear();
        workingdecodons.clear();
        writing = false;
    } else if (attached && !writing && ribcurrent == "BegPro") {
        writing = true;
        workingmarker = codons[ribcursor+1];
        cooldown = 1;
    } else if (attached && !writing && ribcurrent == "Ready ") {
        readiedmarkers.push_back(codons[ribcursor]);
    } else if (attached && !writing && ribcurrent == "RunPro") {
        cooldown = 4;
    } else if (attached && !writing && ribcurrent == "RibJmp") {
        cooldown = 4;
    } else if (attached && !writing && ribcurrent == "Commnt") {
        commenting = true;
    } else if (attached && writing) {
        workingcodons.push_back(codons[ribcursor]);
        workingdecodons.push_back(decodons[ribcursor]);
    } else if (writing && !attached) {
        cout << "??????" << endl;
    }
}

// DNAsm definition
void DNAsm::display() {
    cout << "Codons/Decodons: " << endl;
    for (int i = 0; i < length; i++) {
        cout << "#" << i << ": 0b" << bitset<6>(codons[i]) << " - " << decodons[i] << endl;
    }
}

void DNAsm::display_enzymes(vector<char> markers, map<char, Enzyme> enzymes) {
    if (!markers.empty()) {
         for (uint32_t i = 0; i < (markers.size()); i++) {
             cout << "\nEnzyme #" << i << " with marker 0b" << bitset<6>(markers[i]) << endl;
             for (uint32_t j = 0; j < enzymes.at(markers[i]).decodons.size(); j++) {
                 cout << "#" << j << ": 0b" << bitset<6>(enzymes.at(markers[i]).codons[j]) << " - " << enzymes.at(markers[i]).decodons[j] << endl;
             }
         }
    } else {
        cout << "No enzymes yet." << endl;
    }
}

void DNAsm::open(string filename) {
    length = 0;
    // read file
    ifstream file(filename, ios::binary | ios::in);
    // open file as array of bytes
    if (file.is_open()) {
        file.seekg(0, file.end);
        length = file.tellg();
        file.seekg(0, file.beg);
        codons = new char[length];
        file.read(codons, length);
        file.close();
    } else {
        throw std::invalid_argument("Unable to open file; It either doesn't exist, or this program doesn't have the permissions.");
    }
}

void DNAsm::decode() {
    bool attached = false;
    bool writing = false;
    bool commenting = false;
    // determine purpose of each codon in (codon, six character string) pairs
    decodons = new string[length];
    // loop over codons and determine purpose
    for (int i = 0; i < length; i++) {
        if (codons[i] == 0b111110) {
            attached = 1;
            decodons[i] = "Attach";
        } else if (commenting) {
            if (codons[i] == 0b101101) { commenting = false; }
        } else if (codons[i] == 0b110000 && attached) {
            writing = 1;
            decodons[i] = "BegPro";
            decodons[i+1] = "Marker";
            i += 1;
        } else if (codons[i] == 0b000011 && attached) {
            writing = 0;
            decodons[i] = "EndPro";
        } else if (codons[i] == 0b011111 && attached) {
            attached = 0;
            decodons[i] = "Detach";
        } else if (codons[i] == 0b011110 && attached) {
            decodons[i] = "RibJmp";
            decodons[i+1] = "Arg 1 ";
            decodons[i+2] = "Arg 2 ";
            decodons[i+3] = "Arg 3 ";
            decodons[i+4] = "Arg 4 ";
            i += 4;
        } else if (attached && !writing && codons[i] == 0b001100) {
            decodons[i] = "RunPro";
            decodons[i+1] = "Arg 1 ";
            decodons[i+2] = "Arg 2 ";
            decodons[i+3] = "Arg 3 ";
            decodons[i+4] = "Arg 4 ";
            i += 4;
        } else if (attached && !writing && codons[i] == 0b101101) {
            decodons[i] = "Commnt";
            commenting = true;
        } else if (attached && !writing) {
            decodons[i] = "Ready ";
        } else if (attached && writing && codons[i] == 0b001111) {
            decodons[i] = "CurJmp";
            decodons[i+1] = "Arg 1 ";
            decodons[i+2] = "Arg 2 ";
            decodons[i+3] = "Arg 3 ";
            decodons[i+4] = "Arg 4 ";
            i += 4;
        } else if (attached && writing && codons[i] == 0b111100) {
            decodons[i] = "InsJmp";
            decodons[i+1] = "Arg 1 ";
            decodons[i+2] = "Arg 2 ";
            decodons[i+3] = "Arg 3 ";
            decodons[i+4] = "Arg 4 ";
            i += 4;
        } else if (attached && writing && codons[i] == 0b110011) {
            decodons[i] = "Substi";
            decodons[i+1] = "Arg 1 ";
            decodons[i+2] = "Arg 2 ";
            i += 2;
        } else if (attached && writing && codons[i] == 0b100101) {
            decodons[i] = "Replac";
            decodons[i+1] = "Arg 1 ";
            i += 1;
        } else if (attached && writing && codons[i] == 0b111000) {
            decodons[i] = "StpFwd";
        } else if (attached && writing && codons[i] == 0b000111) {
            decodons[i] = "StpBwd";
        } else if (attached && writing && codons[i] == 0b000100) {
            decodons[i] = "Output";
            decodons[i+1] = "Arg 1 ";
            i++;
        } else if (attached && writing && codons[i] == 0b000000) {
            decodons[i] = "Blank ";
        } else {
            decodons[i] = "      ";
        }
    }
}

// this will setup dnasm, and will be executed in scope of ofApp.cpp
void DNAsm::setup(string filename) {
    time = 0;
    running = true;
    DNAsm::open(filename);
    DNAsm::decode();

    enzymes.clear();
    markers.clear();
    readiedmarkers.clear();
    runningmarkers.clear();
    attached = 0;
    writing = 0;
    commenting = 0;
    ribcursor = 0;
    workingcodons.clear();
    workingdecodons.clear();
    cooldown = 0;
}

void DNAsm::step() {
    if (running && time < 2147483647 && ribcursor < (length - 1)) {
        for (char currentmarker : runningmarkers) {
            Enzyme currentenzyme = enzymes.at(currentmarker);
            if (currentenzyme.instrptr >= currentenzyme.decodons.size() || currentenzyme.cursor >= length) { 
                runningmarkers.erase(remove(runningmarkers.begin(), runningmarkers.end(), currentmarker), runningmarkers.end());
            } else {
                codons = enzymes.at(currentmarker).step(codons);
            }
            decode();
        }
    // then execute the ribosomes stuff
    ribstep();
    time++;
    ribcursor++;
    } else {
        running = false;
    }
}