#include <StonesThrow.h>

#include "WProgram.h"
void setup();
void loop();
StonesThrow st;

#define myID 2
#define pairWithID 1

void setup(){
  st.begin(myID, pairWithID);
}


void loop(){
   
  st.remoteDigitalWrite(11,HIGH);
  delay(1000);
  st.remoteDigitalWrite(11,LOW);
  delay(1000);
   
 
}


int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

