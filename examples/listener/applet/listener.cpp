#include <StonesThrow.h>

// define local and remote arduinos
// these could be entered directly in the begin() call

#define myID 1
#define pairWithID 2

#include "WProgram.h"
void setup();
void loop();
StonesThrow st;

void setup(){
  
  st.begin(myID,pairWithID);
 
  
}

void loop(){
  
  st.update();
    
  delay(10);

}


int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

