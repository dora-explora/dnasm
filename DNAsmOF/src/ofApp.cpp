#include "ofApp.h"
#include "ofxGui.h"
#include "dnasm.h"

DNAsm dnasm;

ofxPanel gui; // entire gui panel
ofxIntField position; // viewports position
ofxToggle play; // whether or not the program is automatically running
ofxToggle follow; // whether or not to follow the ribosome in the program
bool help; // whether or not to display the help text
ofxFloatField speed; // the speed of the program in steps per second

int width; // number of squares that could fit on screen; ceil(width of window / 100)

ofxLabel test1;
ofxLabel test2;

void bytecolor(char byte) {
    int red  = 0;
    int green = 0;
    int blue = 0;
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

void open() {
    ofFileDialogResult file = ofSystemLoadDialog("Load DNAsm file");   
    if(file.bSuccess) {
        dnasm.setup(file.getPath());
        ofBackground(255);
        position.operator=(0);
        play.operator=(false);
        follow.operator=(false);
        speed.operator=(10);
    } else {
        // end the program??????????
    }
}

void setwidth(int w) {
    width = std::ceil(w / 100) + 1;
    if (width > (dnasm.length - position)) { width = (dnasm.length - position); }
}

//--------------------------------------------------------------
void ofApp::setup(){

	open();

    gui.setup();
    gui.add(position.setup("position:", 0, 0, dnasm.length - 10));
    gui.add(play.setup("play", false));
    gui.add(follow.setup("follow ribosome", false));
    gui.add(speed.setup("speed:", 10, 1, 100));
    gui.add(test1.setup("",""));
    gui.add(test2.setup("",""));

    help = true;
    ofBackground(255);
    width = 10;
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
    if (ofGetElapsedTimeMillis() >= (1000 / speed) && play) {
        ofResetElapsedTimeCounter();
        dnasm.step();
    }
    if (follow && dnasm.ribcursor > 4 && dnasm.ribcursor < (dnasm.length - width/2)) {
        position = (dnasm.ribcursor - 4);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    float halfway = ofGetHeight()/2;
    // draw codon squares
    for (int i = 0; i < width; i++) {
        ofSetColor(0);
        ofDrawRectRounded(18 + (i * 100), halfway - 2, 64, 64, 10);
        ofDrawBitmapString(dnasm.decodons[i + position], 28 + (i * 100), halfway + 80);
        bytecolor(dnasm.codons[i + position]);
        ofDrawRectRounded(20 + 5 + (i * 100), halfway + 5, 50, 50, 5);
    }
    // draw enzyme triangles
    for (uint32_t i = 0; i < dnasm.runningmarkers.size(); i++) {
        Enzyme currentenzyme = dnasm.enzymes.at(dnasm.runningmarkers[i]);
        int currentposition = (currentenzyme.cursor - position);
        if (currentposition < width && currentposition >= 0) {
            ofSetColor(0);
            ofDrawTriangle(25 + (currentposition * 100), halfway - 50, 75 + (currentposition * 100), halfway - 50, 50 + (currentposition * 100), halfway - 10);
            ofDrawBitmapString(currentenzyme.decodons[currentenzyme.instrptr], 25 + (currentposition * 100), halfway - 55);
            bytecolor(dnasm.runningmarkers[i]);
            ofDrawTriangle(32 + (currentposition * 100), halfway - 46, 68 + (currentposition * 100), halfway - 46, 50 + (currentposition * 100), halfway - 17);
        }
    }
    // draw ribosome dot
    ofSetColor(0);
    ofDrawCircle(50 + ((dnasm.ribcursor - position) * 100), halfway + 30, 10);
    ofSetColor(255);
    ofDrawCircle(50 + ((dnasm.ribcursor - position) * 100), halfway + 30, 5);
    gui.draw(); 

    ofSetColor(0);
    if (help) { ofDrawBitmapString("Press 'o' to open a file, 'p' to play, 'f' to follow,\nspace to step once, 'a' and 'd' to scroll left and right,\n'w' and 's' to change the speed by 5 steps per second,\nand press 'h' to toggle this text.", 230, 30); }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    switch (key) {
    case 'o':
        open();
        break;
    case 'p':
        if (play) { play.operator=(false); }
        else if (!play) { play.operator=(true); }
        break;
    case 'f':
        if (follow) { follow.operator=(false); }
        else if (!follow) { follow.operator=(true); }
        break;
    case 'h':
        if (help) { help = false; }
        else if (!help) { help = true; }
        break;
    case ' ':
        // test1.operator=(" ");
        dnasm.step();
        break;
    
    case 'a':
        // test1.operator=("a");
        position.operator=(position - 1);
        if (position < 0) { position.operator=(0); }
        setwidth(ofGetWidth());
        // ofSetWindowPosition(ofGetWindowPositionX() - 100, ofGetWindowPositionY()); // this is for fun :3
        break;
    case 'd':
        // test1.operator=("d");
        position.operator=(position + 1);
        if (position > (dnasm.length - width)) { 
            position.operator=(dnasm.length - width); 
            // ofSetWindowShape((width * 100), ofGetHeight());
        }
        // ofSetWindowPosition(ofGetWindowPositionX() + 100, ofGetWindowPositionY()); // this too :3
        break;

    case 'w':
        // test1.operator=("w");
        speed.operator=(speed + 5);
        if (speed > 100) { speed.operator=(100); }
        break;
    case 's':
        // test1.operator=("s");
        speed.operator=(speed - 5);
        if (speed <= 0) { speed.operator=(1); }
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
    setwidth(w);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) { 

}
