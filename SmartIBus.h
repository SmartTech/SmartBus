/*
 * Simple interface to the Fly Sky IBus RC system.
 */

#include <inttypes.h>
#include <Arduino.h>

#define IBUS_BAUD        115200
#define IBUS_CHANNELS        10

class SmartIBus
{
	
	public:

		SmartIBus();
		SmartIBus(uint32_t _baud);
		SmartIBus(HardwareSerial* _serial, uint32_t _baud = IBUS_BAUD);
		SmartIBus(USBSerial*      _serial, uint32_t _baud = IBUS_BAUD);
		
		void begin();
		void begin(uint32_t _baud);
		void handle(void);
		
		uint16_t read(uint8_t channelNr);

	private:
	
		enum State
		{
		GET_LENGTH,
		GET_DATA,
		GET_CHKSUML,
		GET_CHKSUMH,
		DISCARD,
		};

		static const uint8_t PROTOCOL_LENGTH    = 0x20;
		static const uint8_t PROTOCOL_OVERHEAD  = 3; // <len><cmd><data....><chkl><chkh>
		static const uint8_t PROTOCOL_TIMEGAP   = 3; // Packets are received very ~7ms so use ~half that for the gap
		static const uint8_t PROTOCOL_COMMAND40 = 0x40; // Command is always 0x40

		HardwareSerial* serial    = NULL;
		USBSerial*      usbSerial = NULL;
		

		uint32_t baud = IBUS_BAUD;
		uint8_t  state;
		uint32_t last;
		uint8_t  buffer[PROTOCOL_LENGTH];
		uint8_t  ptr;
		uint8_t  len;
		uint16_t channel[IBUS_CHANNELS];
		uint16_t chksum;
		uint8_t  lchksum;
  
};
