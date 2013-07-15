/*
 * ofSoundMushrooms
 * @author Christina Chu, copyright Flaming Lotus Girls
 * @date July 2013
 *
 * Modified from oF Standard Firmata Example
 * This is a simple example use of ofArduino
 *
 * ofArduino currently only supports the standard Arduino boards
 * (UNO, Duemilanove, Diecimila, NG, and other boards based on the
 * ATMega168 or ATMega328 microcontrollers
 * The Arduio FIO and Arduino Mini should also work.
 * The Arduino MEGA and other variants based on microcontrollers
 * other than the ATMega168 and ATMega328 are not currently supported.
 * 
 * To use this example, open Arduino (preferably Arduino 1.0) and 
 * navigate to File -> Examples -> Firmata and open StandardFirmata.
 * Compile and upload StandardFirmata for your board, then close
 * the Arduino application and run this application.
 *
 *
 */

#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
    
    /////////////////////////////
    // SAMPLE SOUNDS, use this for debugging.
    /////////////////////////////
    
    beats.loadSound("sounds/1085.mp3");
    beats.setVolume(0.75f);
    beats.setMultiPlay(false);
    
    vocals.loadSound("sounds/Violet.mp3");
    vocals.setVolume(0.75f);
    vocals.setMultiPlay(false);

    /////////////////////////////
    // MUSHROOM SOUNDS
    /////////////////////////////
    
    // load Accent loop
    accentLoop.loadSound(accentTrack);
    accentLoop.setVolume(0.75f);
    accentLoop.setMultiPlay(false);
    
    // Load Gurgles
    for (int i =0; i < NUM_GURGLE; i++) {
        string gName = gurgleTracks + ofToString(i+1) + ".wav";
        cout << "gurgle loaded: " << gName << endl;
        gurgleSounds[i].loadSound(gName);
        gurgleSounds[i].setVolume(1.0f);  // 100% volume
        gurgleSounds[i].setMultiPlay(false);
        
    }
    
    // Load Breathing
    for (int j =0; j < NUM_BREATHE; j++) {
        string iName = inhaleExhaleTracks + ofToString(j+1) + ".wav";
        cout << "inhale-exhale loaded: " << iName << endl;
        inhaleExhaleSound[j].loadSound(iName);
        inhaleExhaleSound[j].setVolume(0.25f);  // 50% volume
        inhaleExhaleSound[j].setMultiPlay(false);
    }
    
    //////////////////
    
	ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofBackground(0,0,0);
	font.loadFont("franklinGothic.otf", 20);
    smallFont.loadFont("franklinGothic.otf", 14);
    
    irOneValue = "analog IR #1 pin:";
    irTwoValue = "analog IR #2 pin:";
    irThreeValue = "analog IR #3 pin:";

    
    // ****** Replace the string below with the serial port for your Arduino board
    // you can get this from the Arduino application or via command line
    // for OSX, in your terminal type "ls /dev/tty.*" to get a list of serial devices

    ard.connect("/dev/tty.usbmodem1421", 57600);

	// listen for EInitialized notification. this indicates that
	// the arduino is ready to receive commands and it is safe to
	// call setupArduino()
    
	ofAddListener(ard.EInitialized, this, &testApp::setupArduino);
	bSetupArduino	= false;
    // flag so we setup arduino when its ready, you don't need to touch this :)

}

float timeElasped = 0;
float cycleTime = 4; // 4 seconds cycle time for random number generator to update
float breathespeed = 1.0f;


//--------------------------------------------------------------
void testApp::update(){

	updateArduino();
    ofSoundUpdate();
    
    int currentTime = ofGetElapsedTimef();
    
    if (currentTime > (timeElasped+cycleTime)) {
        // slow down the random number generator to prevent audio glitching
        randomGurgle = ofRandom(0,NUM_GURGLE);
        randomSound = ofRandom(0,NUM_BREATHE);
        timeElasped = currentTime;
        cout << "Random Sound# " << randomSound << " Random Gurgle #" << randomGurgle << endl;
    }
    // slow down the update of the random gurgle
    
    inhaleExhaleSound[randomSound].setSpeed(breathespeed);
    inhaleExhaleSound[randomSound].setMultiPlay(false);
    inhaleExhaleSound[randomSound].play();
    
}


// Sensors #1,2,3 should affect breathing rate, gurgles across the board
// but they are separated here for testing purposes.

void testApp:: irOneSound(int dist) {
    
    breathespeed = 1.0f;
    
    // this is a sample, update accordingly to your build
    
   if (dist > 150) {  // update the distance here based on sensor
        breathespeed = 0.75f;
    }
    if (dist > 250) {
        breathespeed = 1.0f;
    }
    if (dist > 400) {
        breathespeed = 1.5f;
    }
    if (dist > 500) {
        breathespeed = 2.0f;
    }      
}


void testApp:: irTwoSound(int dist) {
    
    // this is a sample, update accordingly to your build
    
    if (dist > 300) {
        // stop previous gurgle
        if (!(gurgleSounds[randomGurgle].getIsPlaying())) {
            gurgleSounds[randomGurgle].play();
            cout << "IR #2: play Gurgle #" << randomGurgle << endl;
        }
    }
}

void testApp:: irThreeSound(int dist) {
    
    // this is a sample, update accordingly to your build
    
    
    if (dist > 300) {
        if (!vocals.getIsPlaying()) {  // update this to whatever appropriate sound
            vocals.play();
            cout << "IR#3: vocals playing" << endl;
        }
    }
}


//--------------------------------------------------------------
void testApp::setupArduino(const int & version) {
	
	// remove listener because we don't need it anymore
	ofRemoveListener(ard.EInitialized, this, &testApp::setupArduino);
    
    // it is now safe to send commands to the Arduino
    bSetupArduino = true;
    
    // print firmware name and version to the console
    cout << ard.getFirmwareName() << endl; 
    cout << "firmata v" << ard.getMajorFirmwareVersion() << "." << ard.getMinorFirmwareVersion() << endl;
        
    // Note: pins A0 - A5 can be used as digital input and output.
    // Refer to them as pins 14 - 19 if using StandardFirmata from Arduino 1.0.
    // If using Arduino 0022 or older, then use 16 - 21.
    // Firmata pin numbering changed in version 2.3 (which is included in Arduino 1.0)
    
    // set pin A0 to analog input - IR SENSOR #1
    ard.sendAnalogPinReporting(0, ARD_ANALOG);
    
    // set pin A1 to analog input  - IR SENSOR #2
    ard.sendAnalogPinReporting(1, ARD_ANALOG);

    // set pin A2 to analog input
    
    // - UNCOMMENT LINE BELOW for - IR SENSOR #3
    //ard.sendAnalogPinReporting(2, ARD_ANALOG);

    
    // set pin D13 as digital output
	ard.sendDigitalPinMode(13, ARD_OUTPUT);
      
    // Listen for changes on the digital and analog pins
    ofAddListener(ard.EDigitalPinChanged, this, &testApp::digitalPinChanged);
    ofAddListener(ard.EAnalogPinChanged, this, &testApp::analogPinChanged);    
}

//--------------------------------------------------------------
void testApp::updateArduino(){

	// update the arduino, get any data or messages.
    // the call to ard.update() is required
	ard.update();
	
	// do not send anything until the arduino has been set up
	if (bSetupArduino) {
        // fade the led connected to pin D11
		ard.sendPwm(11, (int)(128 + 128 * sin(ofGetElapsedTimef())));   // pwm...
	}

}

// digital pin event handler, called whenever a digital pin value has changed
// note: if an analog pin has been set as a digital pin, it will be handled
// by the digitalPinChanged function rather than the analogPinChanged function.

//--------------------------------------------------------------
void testApp::digitalPinChanged(const int & pinNum) {
}

// analog pin event handler, called whenever an analog pin value has changed

//--------------------------------------------------------------
void testApp::analogPinChanged(const int & pinNum) {
    // do something with the analog input. here we're simply going to print the pin number and
    // value to the screen each time it changes
    
    int distance = ard.getAnalog(pinNum);
    
    if (pinNum == 0) {
        irOneValue = "IR 1 pin:" + ofToString(pinNum) + " = " + ofToString(ard.getAnalog(pinNum));
        irOneSound(distance);
    }
    if (pinNum == 1) {
        irTwoValue = "IR 2 pin:" + ofToString(pinNum) + " = " + ofToString(ard.getAnalog(pinNum));
        irTwoSound(distance);
    }
    if (pinNum == 2) {
        irThreeValue = "IR 3 pin:" + ofToString(pinNum) + " = " + ofToString(ard.getAnalog(pinNum));
        irThreeSound(distance);
    }

    // plays back continously and has no interactivity
    if(!accentLoop.getIsPlaying()) {
        accentLoop.setVolume(0.25f);
        accentLoop.play();
    }
    
}


//--------------------------------------------------------------
void testApp::draw(){
        
	if (!bSetupArduino){
        font.drawString("arduino not ready...\n", 15, 40);
	} else {
        font.drawString(irOneValue, 15, 80);
        font.drawString(irTwoValue, 15, 120);
        font.drawString(irThreeValue, 15, 160);

        ofSetColor(255, 255, 255);
	}
    
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}