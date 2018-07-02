#ifndef _SMART_PPM_
#define _SMART_PPM_

#include <Arduino.h>

// Minimum and maximum values of PPM signal
#define PPM_MIN              1000
#define PPM_MAX              2000

// The maximum error (in either direction) in channel value
// with which the channel value is still considered valid
#define PPM_MAX_ERROR          10

// The minimum value (time) after which the signal frame is considered to
// be finished and we can start to expect a new signal frame.
#define PPM_BLANK_TIME       2100

class SmartPPM {

    private:
	
		// The pin from which to listen for interrupts
		uint8_t interruptPin = 0;

		// The amount of channels to be expected from the PPM signal.
		uint8_t channelAmount = 0;

		// Arrays for keeping track of channel values
		unsigned long *rawValues = NULL;
		unsigned long *validValues = NULL;

		// A counter variable for determining which channel is being read next
		uint8_t pulseCounter = 0;

		// A time variable to remember when the last pulse was read
		unsigned long microsAtLastPulse = 0;


    public:

		SmartPPM(uint8_t pin, uint8_t channels);
		~SmartPPM();

		uint16_t getRaw(uint8_t channel);

		uint16_t getValid(uint8_t channel);

		void handlePPM();

};

#endif // _SMART_PPM_
