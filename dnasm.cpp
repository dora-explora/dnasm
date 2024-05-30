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

    Enzyme(vector<char> incodons, vector<string> indecodons) {
        codons = incodons;
        decodons = indecodons;
        length = incodons.size() - 1;
        cursor = 0;
        instrptr = 0;
    }

    int run() {
        int offset = 0;
        string current = decodons[instrptr];
        if (current == "JmpCur") {
            cursor = codons[instrptr + 1];
            cout << bitset<6>(cursor);
            offset++;
        } else if (current == "JmpIns") {
            
        } else if (current == "") {
            
        } else if (current == "") {
            
        } else if (current == "") {

        } else if (current == "") {

        } else if (current == "") {

        }
        return offset;
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
    // print file contents as binary
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
        if (codons[i] == 0b000000) {
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
        } else if (codons[i] == 0b111111 && attached) {
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
        } else if (attached && writing && codons[i] == 0b011111) {
            decodons[i] = "Execut";
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
            workingcursor |= codons[i+1] << 24;
            workingcursor |= codons[i+2] << 16;
            workingcursor |= codons[i+3] << 8;
            workingcursor |= codons[i+4];
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