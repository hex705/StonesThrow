/*

StonesThrow.cpp  -- Library for skipping data with xBee mesh networks
Created by Steve Daniels, June 2009.
	
StonesThrow vA001 is released to the public domain under a creativecommons.org 
Attribution-Share Alike license, 3.0 or later.

http://creativecommons.org/licenses/by-sa/3.0/

StonesThrow is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
	
*/

#include "Wprogram.h"
#include "HardwareSerial.h"

#include "StonesThrow.h"


StonesThrow::StonesThrow();


void StonesThrow::begin( HardwareSerial &s )
{
	
	 int src = 1;
	 int dst = 1;
	 // pass settings to next function 
	 StonesThrow::begin ( s , src, dst ); 
	
}


void StonesThrow::begin( HardwareSerial &s, int src, int dst)
{
	 serial = s;
	 setSrcNode(src);  // user node identifier
	 setDstNode(dst);  // user node identifier
	 init();
}

void StonesThrow::init(void)
{
	
	type = -1;
	highByte = 0;
	lastPackageSize = -1;
	packageComplete = false;
	verbose = false;
	if (serial) { 
		serial->println("stoneThrow Inititalized");	
	}
	delay(1500);
	
}


// node declareations
void StonesThrow::setSrcNode(int nodeName){
   //strcpy(srcNode, nodeName);
//	constrain(nodeName,1,100);
	srcNode = nodeName;
}

void StonesThrow::setDstNode(int nodeName){
   //strcpy(dstNode, nodeName);
	dstNode = nodeName;
//	constrain(dstNode,1,200);
}

int StonesThrow::getSrcNode(void){
	return srcNode; 
}

int StonesThrow::getDstNode(void){
	return dstNode;
}

void StonesThrow::verboseStart(){
	verbose = true;
}

void StonesThrow::verboseStop(){
	verbose = false;
}

void StonesThrow::update(){
	// read the serial port and go get the data from it if some have arrived
	int size;	
	if ( (size = serial->available()) > 0 ){
	    readPackage();
		displayPackage();
		executePackage();
	  }
	
//	if (verbose){
//		serial->println("update complete");
//	}

}

void StonesThrow::sendPackage(int type, int pin, int lowByte, int highByte){

  int _type = type;
  int _pin = pin;
  int _lowByte = lowByte;
  int _highByte = highByte;

  serial->print(START_BYTE, BYTE);  
  //serial->print(DELIMITER, BYTE);        
  
  serial->print(srcNode);               
  serial->print(DELIMITER, BYTE);       
  
  serial->print(dstNode); 
  serial->print(DELIMITER, BYTE);    

  serial->print(_type,DEC);         
  serial->print(DELIMITER, BYTE);       

  serial->print(_pin,DEC); 
  serial->print(DELIMITER, BYTE);      
  
  serial->print(_lowByte,DEC);   
  serial->print(DELIMITER, BYTE);     

  serial->print(_highByte,DEC);   
  serial->print(DELIMITER, BYTE);     
 
  serial->println(END_BYTE,BYTE);  

} //end of message

void StonesThrow::readPackage(void){

  int inByte = serial->read();

  if(inByte == START_BYTE){
    
    char tempBuffer[TEMP_BUFFER_LENGTH]; // note this must be char to use atoi later in code
	packageComplete = false;
    int tempIndex = 0;
    int packageIndex = 0;

    while (inByte != END_BYTE){

     if (inByte >= '0' && inByte <= '9')
     {
        tempBuffer[tempIndex] = inByte;
        tempIndex ++;
     } 
     else if (inByte == ',')
     { 
        packageBuffer[packageIndex]= atoi(tempBuffer);
        packageIndex++;
        for (int tB = 0; tB < TEMP_BUFFER_LENGTH; tB ++){
          tempBuffer[tB] = 0;
        }
        tempIndex = 0;
      } // else if
      
      inByte = serial->read();  // reset the conditional for while 
    } // end while
    
    lastPackageSize = packageIndex;

	if (verbose){
		serial->print("lastPackageSize: ");
		serial->println(lastPackageSize);
	}
	
	
	if(lastPackageSize == 6){
    	packageComplete = true; 
	}
  }

}

void StonesThrow::executePackage(){

  if(packageBuffer[1] == srcNode){  // confirm that message is directed to this NODE

    if(packageComplete){ // confirm that a full package was received -- don't count on this as an error check

      int _r_type = packageBuffer[2];
      int _r_pin = packageBuffer[3];
      int _r_lowByte = packageBuffer[4];

      switch(_r_type){

      case DIGITAL_READ:

        {
          pinMode(_r_pin,INPUT);
          int lowByte = digitalRead(_r_pin);
          
		  serial->flush();
          sendPackage(8,_r_pin,lowByte,1);
		 
          break;
        }
      case DIGITAL_WRITE:

        {
          pinMode(_r_pin,OUTPUT);
          digitalWrite(_r_pin,_r_lowByte);
          break;
        }

      case ANALOG_READ:

        {
         
          int temp = analogRead(_r_pin);
          int highByte = temp >> 8;
          int lowByte = temp & B11111111;
          
		  serial->flush();
          sendPackage(9,_r_pin,lowByte,highByte);
          
          break;
        }

      case ANALOG_WRITE:
        {
          pinMode(_r_pin,OUTPUT);
          analogWrite(_r_pin, _r_lowByte);
          break;
        }

      case EVENT_WRITE:
        {
          
          // not implimented yet
          break;

        }

      } // end switch
    }  // end packageComplete

 // empty buffer after message executed
	for (int i = 0; i <= 7; i++){
		packageBuffer[i] = 0;
	}
	serial->flush();
  } // end packageBuffer[1]

}  // end execute package

void StonesThrow::displayPackage(void){
 if (verbose == true){
  if(packageBuffer[1] == srcNode){
    
    if(packageComplete){

      serial->print(srcNode);
      serial->print(", displaying received data: ");

      for (int i = 0; i < lastPackageSize; i++){
        serial->print(packageBuffer[i],DEC);
        serial->print("  ");
      }

      serial->println();
     
    }  // end packageComplete
  } // end packageBuffer[1]
} //if versbose
}  // end packageDisplay


//  REMOTE read and write functions
void StonesThrow::remoteDigitalWrite(int remotePin, int targetState){
 
  // Set type
  type = DIGITAL_WRITE;
  
  // send package over network
  int lowByte = targetState;
  //int highByte = 0; // not needed for digital
  sendPackage(type, remotePin, lowByte, highByte);

}

void StonesThrow::remoteAnalogWrite(int remotePin, int targetState){
	
 // for docs this does not ENSURE you are targetting a PWM pin

  	// Set TYPE
  	type = ANALOG_WRITE;
 	
	// int lowByte = targetState & B11111111;  // LSbyte  add to high byte for val
  	// int highByte = targetState >> 8;        // MSByte  mult by 256
	int lowByte = targetState;
	
	//int hightByte = 0;
  	// send package over network
  sendPackage(type, remotePin, lowByte, highByte);
}

int StonesThrow::remoteDigitalRead(int remotePin){

	serial->flush();
  // Set TYPE
  type = DIGITAL_READ;
  sendPackage(type,remotePin, 0, 0);
  
// wait for reply
  long start = millis();
  long elapsed = 0;
  	boolean timeOut = false;
	packageComplete = false;
	
    int size;
	while (!timeOut && !packageComplete){
			
		if ( (size = serial->available()) > 0 ){
			readPackage();
			displayPackage();
		}  // end serial if
		
		elapsed = millis() - start;
	    if (elapsed >= TIME_OUT){
	      timeOut = true;
		  return -1;
	    } // end elapsed if
	
	} // end while
// end wait for reply
	
    if (packageComplete == true) {
      
	    int val = packageBuffer[4];
        return val;  // the requested digital value
      
    } else {

	   return -2;
		
	}

  

}

int StonesThrow::remoteAnalogRead(int remotePin){

  serial->flush();
  // Set TYPE
  type = ANALOG_READ;
  
  sendPackage(type,remotePin, 0,0);
  
  // wait for reply
  long start = millis();
  long elapsed = 0;
  	boolean timeOut = false;
	packageComplete = false;

    int size;
	while (!timeOut && !packageComplete){
		
		if ( (size = serial->available()) > 0 ){
			readPackage();
			displayPackage();
		}  // end serial if

		elapsed = millis() - start;
	    if (elapsed >= TIME_OUT){
	      timeOut = true;
		return -1;
	    } // end elapsed if

	} // end while
	
// end wait for reply

    if (packageComplete == true) {  
	 
      int val =  packageBuffer[4] + packageBuffer[5]*256;
      return val;  // the requested analog value    
    
    } else {
	
		return -2;
	
	}


}



void StonesThrow::myDigitalBlink(int pin){
		_pin = pin;
		pinMode(_pin,OUTPUT);
		for (int i =0 ; i <3;i++){  
		digitalWrite(_pin,HIGH);
		delay(1000);
		digitalWrite(_pin,LOW);
		delay(1000);
}

}
