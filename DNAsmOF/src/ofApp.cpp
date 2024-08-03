#include "ofApp.h"
#include "ofxGui.h"
#include "dnasm.h"

ofxPanel gui;
ofxIntSlider position;
ofxIntSlider speed;

//--------------------------------------------------------------
void ofApp::setup(){
    gui.setup();
    gui.add(position.setup("position", 0, 0, 300));
    gui.add(speed.setup("speed", 10, 0, 100));

    ofBackground(255);
    ofSetWindowTitle("DNAsm");
    ofSetFrameRate(60);

	DNAsm dnasm;
	dnasm.open();
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
    ofDrawRectRounded(120, ofGetHeight()/2, 60, 60, 15);
    ofDrawRectRounded(220, ofGetHeight()/2, 60, 60, 15);
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
