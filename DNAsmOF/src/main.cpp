#include "ofMain.h"
#include "ofApp.h"
#include "dnasm.h"

//========================================================================
int main(){

	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLWindowSettings settings;
	settings.windowMode = OF_WINDOW;
	settings.setSize(1300, 400);

	auto window = ofCreateWindow(settings);

	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();

}
