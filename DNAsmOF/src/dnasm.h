#ifndef DNASM_H
#define DNASM_H

#include "ofMain.h"
#include "ofApp.h"
#include "ofxGui.h"

class DNAsm {
    public:
	    char* codons;
		string* decodons;
        int time;
        int length;
        void open(string filename);
        void decode();
        void setup();
        void step();
        // stuff like step(), setup(), public variables...
};

#endif //DNASM_H
