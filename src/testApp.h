#pragma once

#include "ofMain.h"
#include "ofEvents.h"

#define NUM_GURGLE 4
#define NUM_ACCENT 1
#define NUM_BREATHE 10


class testApp : public ofBaseApp{

public:

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);

	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	ofImage				bgImage;
	ofTrueTypeFont		font;
    ofTrueTypeFont      smallFont;
	ofArduino	ard;
	bool		bSetupArduino;			// flag variable for setting up arduino once

    
    ofSoundPlayer  beats;
    ofSoundPlayer  synth;
    ofSoundPlayer  vocals;

    
private:
    
    ///////// Mushroom Sounds ///////////
    int randomGurgle;
    int randomBreathe;
    int randomSound;

    
    ofSoundPlayer  accentLoop;
    ofSoundPlayer  inhaleExhaleSound[NUM_BREATHE];
    ofSoundPlayer  gurgleSounds[NUM_GURGLE];

    string accentTrack = "sounds/normalized/scrapes_loop.wav";
    string gurgleTracks = "sounds/volume_adjusted/gurgle_";
    string inhaleExhaleTracks = "sounds/normalized/inhale_exhale_";
    
    void irOneSound(int dist);
    void irTwoSound(int dist);
    void irThreeSound(int dist);

    string irOneValue;
    string irTwoValue;
    string irThreeValue;
    
    void setupArduino(const int & version);
    void digitalPinChanged(const int & pinNum);
    void analogPinChanged(const int & pinNum);
	void updateArduino();
    
};

