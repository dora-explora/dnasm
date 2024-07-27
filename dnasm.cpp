// This program runs DNAsm code from the DNAsm.bin file. 

#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>
#include <string>
#include <bitset>
#include <tuple>

using namespace std;

string* decodons; // all decodons in strings (human readable and properly identified)
char* codons; // all codons in chars (single bytes)
int length; // length of file in bytes (codons)

class Enzyme {
    public:
    vector<char> codons; // all the enzymes codons
    vector<string> decodons; // all the enzymes decodons
    int length; // length of enzymes code, starts at 0 (for iterating)
    int cursor; // cursor for where the enzyme is in the code
    int instrptr; // instruction pointer
    char subarg1; // first arg for substitution 
    char subarg2; // second arg for substitution
    int cooldown; // how long to wait for args to be read
    bool backslash; // for printing
    bool substituting; // turned on if subarg1 and subarg2 are set, turns off if they're not set or are actively being set 

    Enzyme(vector<char> incodons, vector<string> indecodons) {
        codons = incodons;
        decodons = indecodons;
        length = incodons.size() - 1;
        cursor = 0;
        instrptr = 0;
        cooldown = 0;
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

    void cursorjump () { 
        cursor = 0;
        cursor |= (codons[instrptr + 1] & 0x3F) << 18;
        cursor |= (codons[instrptr + 2] & 0x3F) << 12;
        cursor |= (codons[instrptr + 3] & 0x3F) << 6;
        cursor |= (codons[instrptr + 4] & 0x3F);
        // cout << "Cursor: " << bitset<24>(cursor) << endl;
    }
    
    void instructionjump () {
        int tempinstrptr = instrptr;
        instrptr = 0;
        instrptr |= (codons[tempinstrptr - 3] & 0x3F) << 18;
        instrptr |= (codons[tempinstrptr - 2] & 0x3F) << 12;
        instrptr |= (codons[tempinstrptr - 1] & 0x3F) << 6;
        instrptr |= (codons[tempinstrptr] & 0x3F);
        // cout << "Instruction pointer: " << bitset<24>(instrptr) << endl;
    }

    char* step(char* globalcodons) {
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
        } else if (current == "OutCur") {
            // yeahhhhhhhhh i need to figure this out
        } 
        instrptr++;
        return globalcodons;
    };
};

map<char, Enzyme> enzymes; // all enzymes and their markers
vector<char> markers; // all markers in order of when their enzymes were written, readied, running, or not.

class Ribosome {
    public:
    vector<char> readiedmarkers; // markers of all readied enzymes
    vector<char> runningmarkers; // markers of all running enzymes
    char recentmarker; // marker of the enzyme that was most recently run
    char workingmarker; // marker of the enzyme currently being transcribed
    bool attached = false; // true if ribosome is currently attached
    bool writing = false; // true if ribosome is currently writing
    bool commenting = false; // true if ribosome is currently commenting
    int ribcursor = 0; // ribosomes position/cursor
    string ribcurrent; // decodon at ribosomes cursor
    vector<char> workingcodons; // codons of ribosomes working enzyme
    vector<string> workingdecodons; // decodons of ribosomes working enzyme
    int cooldown = 0; // cooldown, used for when ribosome has to read bytes for timing but doesn't need to actually do anything with them
    int initialcursor; // where readied enzymes are deployed to at first 

    void runproteins() {
        for (int i = 0; i < readiedmarkers.size(); i++) { 
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

    void ribosomejump() {
        int tempribcursor = ribcursor;
        ribcursor = 0;
        ribcursor |= (codons[tempribcursor-3] & 0x3F) << 18;
        ribcursor |= (codons[tempribcursor-2] & 0x3F) << 12;
        ribcursor |= (codons[tempribcursor-2] & 0x3F) << 6;
        ribcursor |= (codons[tempribcursor] & 0x3F);
    }



    void step() {
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
};

void display() {
    cout << "Codons/Decodons: " << endl;
    for (int i = 0; i < length; i++) {
        cout << "#" << i << ": 0b" << bitset<6>(codons[i]) << " - " << decodons[i] << endl;
    }
}

void display_enzymes(vector<char> markers, map<char, Enzyme> enzymes) {
    if (!markers.empty()) {
         for (int i = 0; i < (markers.size()); i++) {
             cout << "\nEnzyme #" << i << " with marker 0b" << bitset<6>(markers[i]) << endl;
             for (int j = 0; j < enzymes.at(markers[i]).decodons.size(); j++) {
                 cout << "#" << j << ": 0b" << bitset<6>(enzymes.at(markers[i]).codons[j]) << " - " << enzymes.at(markers[i]).decodons[j] << endl;
             }
         }
    } else {
        cout << "No enzymes yet." << endl;
    }
}

void open(string filename) {
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

void decode() {
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
        } else if (attached && writing && codons[i] == 0b000101) {
            decodons[i] = "OutCur";
        } else if (attached && writing && codons[i] == 0b111111) {
            decodons[i] = "Execut";
        } else if (attached && writing && codons[i] == 0b000000) {
            decodons[i] = "Blank ";
        } else {
            decodons[i] = "      ";
        }
    }
}

int main() {
    int time = 0;
    bool finished = false;

    string filename;
    // cout << "Type in the name of your file (ex. DNAsm.bin):\n";
    // cin >> filename;
    filename = "DNAsm.bin";
    open(filename);
    decode();

    Ribosome ribosome;

    // string nothing;
    // main loop!
    while (!finished && time != -2147483647 && ribosome.ribcursor != (length)) {
        // execute all enzymes
        for (char recentmarker : ribosome.runningmarkers) {
            codons = enzymes.at(recentmarker).step(codons);
            decode();
        }
        // then execute the ribosomes stuff
        ribosome.step();
        display();
        cout << "ribosomes cursor: " << (ribosome.ribcursor) << endl;
        cout << "time: " << time << endl;
        // cin >> nothing; // just so time can be stepped manually
        time++;
        ribosome.ribcursor++;
    }
    display();
    display_enzymes(markers, enzymes);
    delete[] codons;
    delete[] decodons;

    return 0;
}