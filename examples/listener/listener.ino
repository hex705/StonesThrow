// Requires a remote NODE

#include <StonesThrow.h>

// define local and remote arduinos
// these are local IDs  -- they are not xBee addresses.
// you must configure your xBee seperately

#define myID 1
#define pairWithID 2

StonesThrow stonesThrow;

void setup(){
  
  Serial.begin(9600);
  stonesThrow.begin(Serial, myID, pairWithID );
 
}

void loop(){
  
  stonesThrow.update();
    
  delay(10);

}

