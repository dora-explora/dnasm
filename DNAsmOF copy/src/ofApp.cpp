#include "ofApp.h"
#include "ofxGui.h"
#include "dnasm.h"

DNAsm dnasm;

ofxPanel gui;
ofxIntField position;
ofxButton play;
ofxIntField speed;

ofxLabel test;


void bytecolor(char byte) {
    int red;
    int green;
    int blue;
    switch (byte & 0b00110000) {
    case 0b00000000:
        red = 0;
        break;
    case 0b00010000:
        red = 85;
        break;
    case 0b00100000:
        red = 170;
        break;
    case 0b00110000:
        red = 255;
        break;
    }
    switch (byte & 0b00001100) {
    case 0b00000000:
        green = 0;
        break;
    case 0b00000100:
        green = 85;
        break;
    case 0b00001000:
        green = 170;
        break;
    case 0b00001100:
        green = 255;
        break;
    }
    switch (byte & 0b00000011) {
    case 0b00000000:
        blue = 0;
        break;
    case 0b00000001:
        blue = 85;
        break;
    case 0b00000010:
        blue = 170;
        break;
    case 0b00000011:
        blue = 255;
        break;
    }
    ofSetColor(red, green, blue);
}

void step() {
    // add finish detection and stuff
    dnasm.step();
    
    dnasm.ribosome.step(dnasm.codons, dnasm.decodons);
    dnasm.time++;
    dnasm.ribosome.ribcursor++;
}

//--------------------------------------------------------------
void ofApp::setup(){

    string filename;
    filename = "../../DNAsm.bin";
	dnasm.setup(filename);

    gui.setup();
    // "Press space to step forward in time,\n\'a\' and \'d\' to step left and right,\nand \'w\' and \'s\' to up the speed by 5."
    gui.add(position.setup("position", 0, 0, dnasm.length - 10));
    gui.add(play.setup("play"));
    gui.add(speed.setup("speed", 10, 0, 100));
    gui.add(test.setup("",""));

    ofBackground(255);
	ofSetCircleResolution(100);

    ofSetWindowTitle("DNAsm");
    ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::update() {
    test.operator=(dnasm.decodons[dnasm.ribosome.ribcursor]);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(0);
    ofNoFill();
    ofSetLineWidth(10);
    for (int i = 0; i < 10; i++) {
        ofDrawRectangle(20 + (i * 100), ofGetHeight()/2, 60, 60);
    }
    ofFill();
    for (int i = 0; i < 10; i++) {
        bytecolor(dnasm.codons[i + position]);
        ofDrawRectangle(20 + 5 + (i * 100), ofGetHeight()/2 + 5, 50, 50);
    }
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    switch (key) {
    case ' ':
        // test.operator=(" ");
        step();
        break;
    
    case 'E': //????????????????????? i have no idea what the keycode for enter is
        test.operator=("ENTER");
        break;
    
    case 'a':
        // test.operator=("a");
        position.operator=(position - 1);
        if (position < 0) { position.operator=(0); }
        break;
    case 'd':
        // test.operator=("d");
        position.operator=(position + 1);
        if (position > (dnasm.length - 10)) { position.operator=(dnasm.length - 10); }
        break;

    case 'w':
        // test.operator=("w");
        speed.operator=(speed + 5);
        if (speed > 100) { speed.operator=(100); }
        break;
    case 's':
        // test.operator=("s");
        speed.operator=(speed - 5);
        if (speed < 0) { speed.operator=(0); }
        break;

    case 't':
        test.operator=(std::to_string(dnasm.markers[0]));
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) { 

}
