#include "SmartIBus.h"

SmartIBus IBus(&Serial1);

void setup() 
{
  IBus.begin();
}

void loop() 
{
  IBus.handle();
  
  static uint32_t lastMillis = 0;
  if(millis() - lastMillis >= 1000)
  {
	  for(uint8_t i=0; i<IBUS_CHANNELS; i++)
	  {
		  Serial.print(IBus.read(i));
		  Serial.print(" ");
	  }
	  Serial.println();
	  lastMillis = millis();
  }
}
