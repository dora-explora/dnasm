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
    map<char, Enzyme> enzymes; // all enzymes and their markers
    vector<char> markers; // all markers in order of when their enzymes were written, readied, running, or not.

    void open(string filename);
    void decode();
    void setup(string filename);
    void step();
    void display();
    void display_enzymes(vector<char> markers, map<char, Enzyme> enzymes);

    vector<char> readiedmarkers; // markers of all readied enzymes, clears on runproteins();
    vector<char> runningmarkers; // markers of all running enzymes
    char recentmarker; // marker of the enzyme that was most recently run
    char workingmarker; // marker of the enzyme currently being transcribed
    bool attached; // true if ribosome is currently attached
    bool writing; // true if ribosome is currently writing
    bool commenting; // true if ribosome is currently commenting
    int ribcursor; // ribosomes position/cursor
    string ribcurrent; // decodon at ribosomes cursor
    vector<char> workingcodons; // codons of ribosomes working enzyme
    vector<string> workingdecodons; // decodons of ribosomes working enzyme
    int cooldown; // cooldown, used for when ribosome has to read bytes for timing but doesn't need to actually do anything with them
    int initialcursor; // where readied enzymes are deployed to at first 

    void runproteins();
    void ribosomejump();
    void ribstep();
};

#endif //DNASM_H
