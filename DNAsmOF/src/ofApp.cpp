#include "ofApp.h"
#include "ofxGui.h"
#include "dnasm.h"

DNAsm dnasm;

ofxPanel gui;
ofxIntField position;
ofxButton play;
ofxIntField speed;

ofxLabel test1;
ofxLabel test2;

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

//--------------------------------------------------------------
void ofApp::setup(){

    string filename;
    filename = "../../stepfwd.bin";
	dnasm.setup(filename);

    gui.setup();
    // "Press space to step forward in time,\n\'a\' and \'d\' to step left and right,\nand \'w\' and \'s\' to up the speed by 5."
    gui.add(position.setup("position", 0, 0, dnasm.length - 10));
    gui.add(play.setup("play"));
    gui.add(speed.setup("speed", 10, 0, 100));
    gui.add(test1.setup("",""));
    gui.add(test2.setup("",""));

    ofBackground(255);
	ofSetCircleResolution(100);
    ofFill();
    ofSetLineWidth(10);

    ofSetWindowTitle("DNAsm");
    ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::update() {
    test1.operator=(dnasm.decodons[dnasm.ribcursor]);
    test2.operator=(std::to_string(dnasm.runningmarkers.size()));
    if (!dnasm.running) { test1.operator=("finished!"); ofBackground(200); }
}

//--------------------------------------------------------------
void ofApp::draw(){
    float halfway = ofGetHeight()/2;
    for (int i = 0; i < 10; i++) {
        ofSetColor(0);
        ofDrawRectRounded(18 + (i * 100), halfway - 2, 64, 64, 10);
        bytecolor(dnasm.codons[i + position]);
        ofDrawRectRounded(20 + 5 + (i * 100), halfway + 5, 50, 50, 5);
    }
    for (int i = 0; i < dnasm.runningmarkers.size(); i++) {
        int currentposition = (dnasm.enzymes.at(dnasm.runningmarkers[i]).cursor - position);
        if (currentposition <= 9 && currentposition >= 0) {
            ofSetColor(0);
            ofDrawTriangle(25 + (currentposition * 100), halfway - 50, 75 + (currentposition * 100), halfway - 50, 50 + (currentposition * 100), halfway - 10);
            bytecolor(dnasm.runningmarkers[i]);
            ofDrawTriangle(32 + (currentposition * 100), halfway - 46, 68 + (currentposition * 100), halfway - 46, 50 + (currentposition * 100), halfway - 17);
        }
    }
    ofSetColor(0);
    ofDrawCircle(50 + ((dnasm.ribcursor - position) * 100), halfway + 30, 10);
    ofSetColor(255);
    ofDrawCircle(50 + ((dnasm.ribcursor - position) * 100), halfway + 30, 5);
    gui.draw(); 
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    switch (key) {
    case ' ':
        // test1.operator=(" ");
        dnasm.step();
        break;
    
    case 'p':
        // play
        break;
    
    case 'a':
        // test1.operator=("a");
        position.operator=(position - 1);
        if (position < 0) { position.operator=(0); }
        break;
    case 'd':
        // test1.operator=("d");
        position.operator=(position + 1);
        if (position > (dnasm.length - 10)) { position.operator=(dnasm.length - 10); }
        break;

    case 'w':
        // test1.operator=("w");
        speed.operator=(speed + 5);
        if (speed > 100) { speed.operator=(100); }
        break;
    case 's':
        // test1.operator=("s");
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
