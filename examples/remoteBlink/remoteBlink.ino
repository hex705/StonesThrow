// Requires a partnering Arduino running listener

// NODES are local IDs  -- they are not xBee addresses.
// you must configure your xBee seperately


#include <StonesThrow.h>

StonesThrow st;

#define myID 2
#define pairWithID 1

void setup(){
  Serial.begin(9600);
  st.begin(Serial, myID, pairWithID);
  
}


void loop(){
   
  st.remoteDigitalWrite(13,HIGH);
  delay(2000);
  st.remoteDigitalWrite(13,LOW);
  delay(500);
   
 
}

