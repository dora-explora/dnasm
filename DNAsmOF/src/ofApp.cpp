#include "ofApp.h"
#include "ofxGui.h"
#include "dnasm.h"

DNAsm dnasm;

ofxPanel gui;
ofxIntField position;
ofxIntField speed;
ofxLabel decodonA;
ofxLabel keypressed;

//--------------------------------------------------------------
void ofApp::setup(){

    string filename;
    filename = "../../DNAsm.bin";
	dnasm.setup(filename);

    gui.setup();
    // "Press space to step forward in time,\n\'a\' and \'d\' to step left and right,\nand \'w\' and \'s\' to up the speed by 5."
    gui.add(keypressed.setup("",""));
    gui.add(position.setup("position", 0, 0, dnasm.length));
    gui.add(speed.setup("speed", 10, 0, 100));
    gui.add(decodonA.setup("decodonA", "please wait.."));

    ofBackground(255);
    ofSetWindowTitle("DNAsm");
    ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(0);
    ofNoFill();
    ofSetLineWidth(5);
    ofDrawRectRounded(20, ofGetHeight()/2, 60, 60, 15);
    // ofDrawRectRounded(120, ofGetHeight()/2, 60, 60, 15);
    // ofDrawRectRounded(220, ofGetHeight()/2, 60, 60, 15);
    // ofDrawRectRounded(320, ofGetHeight()/2, 60, 60, 15);
    // ofDrawRectRounded(420, ofGetHeight()/2, 60, 60, 15);
    // ofDrawRectRounded(520, ofGetHeight()/2, 60, 60, 15);
    // ofDrawRectRounded(620, ofGetHeight()/2, 60, 60, 15);
    // ofDrawRectRounded(720, ofGetHeight()/2, 60, 60, 15);
    // ofDrawRectRounded(820, ofGetHeight()/2, 60, 60, 15);
    // ofDrawRectRounded(920, ofGetHeight()/2, 60, 60, 15);
    decodonA.operator=(dnasm.decodons[0]);
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    switch (key) {
    case ' ':
        keypressed.operator=("SPAAAAAAAACE");
        break;
    
    case 'a':
        position.operator=(position - 1);
        if (position < 0) { position.operator=(0); }
        break;
    case 'd':
        position.operator=(position + 1);
        if (position > dnasm.length) { position.operator=(dnasm.length); }
        break;

    case 'w':
        speed.operator=(speed + 5);
        if (speed > 100) { speed.operator=(100); }
        break;
    case 's':
        speed.operator=(speed - 5);
        if (speed < 0) { speed.operator=(0); }
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
