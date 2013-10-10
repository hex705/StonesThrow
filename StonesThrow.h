/*

StonesThrow.cpp  -- Library for skipping data with xBee mesh networks
Created by Steve Daniels, June 2009.
	
StonesThrow is released to the public domain under a creativecommons.org 
Attribution-Share Alike license, 3.0 or later.

http://creativecommons.org/licenses/by-sa/3.0/

StonesThrow is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
	
*/

#ifndef StonesThrow_h
#define StonesThrow_h


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#include <HardwareSerial.h>


// serial payload delimiters
#define START_BYTE 42   //    42 = * 
#define DELIMITER  44   //    44 = ,  
#define END_BYTE  35   //    35 = # 

// define message TYPEs 
#define DIGITAL_READ 1
#define DIGITAL_WRITE 2
#define ANALOG_READ 3
#define ANALOG_WRITE 4
#define EVENT_WRITE 5

// array sizes
#define TEMP_BUFFER_LENGTH 5   // max size of a chunk
#define PACKAGE_BUFFER_LENGTH 7

#define TIME_OUT 1500  // time to wait for a return value in READ functions

class StonesThrow
{
	
	
public:

	StonesThrow();
	// initialize system
	void begin( HardwareSerial & );
	void begin( HardwareSerial &, int, int );
	void init();
	
	// node labelling 
	void setSrcNode(int);
	void setDstNode(int);
	int getSrcNode();
	int getDstNode();
	void verboseStart();
	void verboseStop();
	
	// read and write remote pins
	void remoteDigitalWrite(int, int);
	void remoteAnalogWrite(int, int);
	int remoteAnalogRead(int);
	int remoteDigitalRead(int);

	
	// package send, recieve
	void update();
	void sendPackage(int, int, int, int);
	void readPackage();
	void executePackage();
	
	void action();
	void displayPackage();
	
	// test function
	void myDigitalBlink(int); 

	
private:
	
	
	HardwareSerial *serial;
	
	int _pin;  // for test function -- CAN DELETE LATER
	
	int  srcNode;  // space for default NI for local arduino, assuming pairwise connection
	int  dstNode;  // space for default NI for remote arduino, assuming pairwise connection
	
	boolean verbose;
	// array sizes for data sending and receiving

	int lastPackageSize;
	boolean packageComplete;
	int packageBuffer[PACKAGE_BUFFER_LENGTH];

	// send variables
	int type;
	int lowByte;
	int highByte;
	
		
}; // end class

# endif   // end #ifndef StonesThrow_h