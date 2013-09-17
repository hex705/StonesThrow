#include <StonesThrow.h>

StonesThrow st;

#define myID 2
#define pairWithID 1

void setup(){
  st.begin(myID, pairWithID);
}

void loop(){
   
  int analogValue = analogRead(0);            // read a local analog pin
  
   analogValue = map(analogValue,0,1023,0,100);  // scale reading to single BYTE
  
  st.remoteAnalogWrite(11, analogValue);      // write scaled reading to remote NODE 

  delay(250);  // cn be smaller but if erratic results emerge -- increase again
 
}

