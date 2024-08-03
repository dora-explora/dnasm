#ifndef DNASM_H
#define DNASM_H

#include "ofMain.h"
#include "ofApp.h"
#include "ofxGui.h"

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

    Enzyme(vector<char> incodons, vector<string> indecodons);
    void print(char rawinput);
    void cursorjump();
    void instructionjump();
    char* step(char* globalcodons);
};

class DNAsm {
    public:
	char* codons;
	string* decodons;
    int time;
    int length;

    void open(string filename);
    void decode();
    void setup(string filename);
    void step();
    void display();
    void display_enzymes(vector<char> markers, map<char, Enzyme> enzymes);
};

#endif //DNASM_H
