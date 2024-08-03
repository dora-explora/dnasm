#include "ofApp.h"
#include "ofxGui.h"
#include "dnasm.h"

ofxPanel gui;
// ofxInputField positioninput;
ofxIntSlider speedslider;
ofxLabel decodonA;

DNAsm dnasm;

//--------------------------------------------------------------
void ofApp::setup(){

    string filename;
    filename = "../../DNAsm.bin";
	dnasm.setup(filename);

    gui.setup();
    // gui.add(positioninput.setup(ofParameter<int>, 100, 20));
    gui.add(speedslider.setup("speed", 10, 0, 100));
    gui.add(decodonA.setup("decodonA", "please wait..", 100, 20));

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
    decodonA.operator=(dnasm.decodons[0]);
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
