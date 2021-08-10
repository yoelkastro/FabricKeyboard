/*
 * FabricKeyboard
 * 
 * Main script. Handles the capacitive touch sensors and the MIDI output
 * Meant to be used with Arduino Nano 33 BLE.
 *
 */

/* TO-DO: Attach to digital pin, read state of USB cable. Dynamic changes. */
#define BLE_DEVICE false

// Include BLE MIDI libraries
#include <BLEMIDI_Transport.h>
#include <hardware/BLEMIDI_nRF52.h>
#include <hardware/BLEMIDI_ArduinoBLE.h>

// Include USB MIDI libraries
#include <Control_Surface.h>

// Include I2C device libraries for capacitive touch sensors and OLED screen
#include "Adafruit_MPR121.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Get the value of a given bit in an integer 
#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

// Define default values for the I2C devices
#define SCREEN_ADDRESS 0x3C
#define NUM_CAPS 4

// Define the I2C devices
Adafruit_MPR121 caps[NUM_CAPS];
Adafruit_SSD1306 display(128, 64, &Wire, 4);

// The values of the keys on the capacitive touch sensors. True if pressed, false otherwise
bool keyVals[NUM_CAPS][12];

// Initialize the MIDI interfaces
USBMIDI_Interface usb;
BLEMIDI_CREATE_DEFAULT_INSTANCE();

// Boolean value representing if the keyboard is currently connected by BLE or not
bool isConnected;

/*
 * Updates the values of the keys of according to the value of one MPR121 sensor.
 * 
 * @param keys: 	The array representing the keys attached to one MPR121. Should be an element of keyVals.
 * @param reading: 	The values read from the associated MPR121.
 */
void getTouches(bool keys[], uint16_t reading){
  for(int i = 0; i < 12; i ++){
    keys[i] = reading & _BV(i);
  }
}

/*
 * Update the values for all keys attached to all capacitive touch sensors.
 */
void updateKeyValues(){
	// Iterate over all the capacitive touch sensors
	for(int i = 0; i < NUM_CAPS; i ++){
		getTouches(keyVals[i], caps[i].touched());
	}
}

/*
 * Update the values for all keys attached to all capacitive touch sensors, and play them.
 * The act of playing the note has to be separately encapsulated since checking if the value has changes
 * is significantly more space efficient when the value is being updated instead of in the main loop.
 */
void updateKeyValuesAndPlay(){
	// Iterate over all the capacitive touch sensors
	for(int c = 0; c < NUM_CAPS; c ++){

		// Read the value of the capacitive touch sensor
		uint16_t reading = caps[c].touched();

		// Iterate over all the keys on the sensor
		for(int i = 0; i < 12; i ++){
			// Get the new value for that key
		  	bool newVal = reading & _BV(i);

		  	// If the value has changed, either turn the note on or off 
		  	// depending on if the key is now pressed or not
		    if(newVal != keyVals[c][i]){
		    	if(newVal)
		    		noteOn(1, c * 12 + i + 36, 127);
		    	else
		    		noteOff(1, c * 12 + i + 36, 127);
		    }

		    // Update the value for the key
		    keyVals[c][i] = newVal;
		  }
	}
}

/*
 * Shows the state of all the keys on the display, where each row is one capacitive touch sensor and each box is one key.
 * The box is filled in if the key is pressed and empty otherwise.
 */
void showKeyStates(){

	display.clearDisplay();

	// Iterate over all the capacitive touch sensors
	for(int i = 0; i < NUM_CAPS; i ++){
		// Iterate over all the keys on the sensor
		for(int j = 0; j < 12; j ++){

			// If the key is pressed, draw the corresponding box filled in
			if(keyVals[i][j])
				display.fillRect((display.width() / 12) * (12 - j), (display.height() / NUM_CAPS) * i, (display.width() / 16), (display.width() / 16), SSD1306_WHITE);
			// Otherwise, draw the box empty  
			else
				display.drawRect((display.width() / 12) * (12 - j), (display.height() / NUM_CAPS) * i, (display.width() / 16), (display.width() / 16), SSD1306_WHITE);
			}
		
	}
	display.display();
}

/*
 * Sends a MIDI note on event over the selected output method.
 * 
 * @param channel:  The MIDI channel the note should be played on.
 * @param note:		The note that should be played.
 * @param velocity:	The velocity of the note.
 * 
 */
void noteOn(byte channel, byte note, byte velocity){
	if(BLE_DEVICE){
		MIDI.sendNoteOn(note, velocity, channel);
	}
	else{
		Control_Surface.sendNoteOn(MIDIAddress(note), velocity);
	}
}

/*
 * Sends a MIDI note off event over the selected output method.
 * 
 * @param channel:  The MIDI channel the note should be turned off on.
 * @param note:		The note that should be turned off.
 * @param velocity:	The velocity of the note.
 * 
 */
void noteOff(byte channel, byte note, byte velocity){
	if(BLE_DEVICE){
		MIDI.sendNoteOff(note, velocity, channel);
	}
	else{
		Control_Surface.sendNoteOff(MIDIAddress(note), velocity);
	}
  
}

/*
 * Change the value of isConnected when the device connects or disconnects
 */
void onConnect(){
	isConnected = true;
}

void onDisconnect(){
	isConnected = false;
}

/*
void OnNoteOn(byte channel, byte note, byte velocity) {}
void OnNoteOff(byte channel, byte note, byte velocity) {}
*/

void setup(){

	display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
	display.clearDisplay();
	display.display();
	
	for(int i = 0; i < NUM_CAPS; i ++){
		caps[i] = Adafruit_MPR121();
		caps[i].begin(0x5A + i);
	}

	if(BLE_DEVICE){
		MIDI.begin();

		BLEMIDI.setHandleConnected(onConnect);
  		BLEMIDI.setHandleDisconnected(onDisconnect);
		
  		/*MIDI.setHandleNoteOn(OnNoteOn);
  		MIDI.setHandleNoteOff(OnNoteOff);*/
	}
	else{
		Control_Surface.begin();
	}
	
}

void loop(){

	// The input has to be read every loop for the connection to work
	if(BLE_DEVICE)
		MIDI.read();

	updateKeyValuesAndPlay();
	showKeyStates();

	/*for(int c = 0; c < NUM_CAPS; c ++){
		for(int i = 0; i < 12; i ++)
	}*/

}
