/*
 * MixedOutputTest
 * 
 * Script for testing the two output methods the keyboard should have: USB and BLE.
 * The BLE_DEVICE constant determines which of these is used: true for BLE and false for USB.
 *
 */

#define BLE_DEVICE true

#include <BLEMIDI_Transport.h>
#include <hardware/BLEMIDI_nRF52.h>
#include <hardware/BLEMIDI_ArduinoBLE.h>

#include <Control_Surface.h>

USBMIDI_Interface usb;
BLEMIDI_CREATE_DEFAULT_INSTANCE();

bool isConnected;

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
void onConnected(){isConnected = true;}
void onDisconnected(){isConnected = false;}
void OnNoteOn(byte channel, byte note, byte velocity) {}
void OnNoteOff(byte channel, byte note, byte velocity) {}
*/

void setup(){

	if(BLE_DEVICE){
		MIDI.begin();

		/*BLEMIDI.setHandleConnected(onConnected);
  		BLEMIDI.setHandleDisconnected(onDisconnected);

  		MIDI.setHandleNoteOn(OnNoteOn);
  		MIDI.setHandleNoteOff(OnNoteOff);*/
	}
	else{
		Control_Surface.begin();
	}
	
}

void loop(){

	if(BLE_DEVICE)
		MIDI.read();
		
	noteOn(1, 48, 127);
	delay(1000);
	noteOff(1, 48, 127);
	delay(1000);

}
