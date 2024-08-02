#include "ofApp.h"
#include "ofxGui.h"
#include "dnasm.h"

ofxPanel gui;
ofxIntSlider position;

//--------------------------------------------------------------
void ofApp::setup(){
    gui.setup();
    gui.add(position.setup("position", 0, 0, 300));

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
    ofDrawRectRounded(120, ofGetHeight()/2, 60, 60, 15);
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
