#include "SmartIBus.h"

SmartIBus::SmartIBus() :
	SmartIBus(&Serial)
{
}

SmartIBus::SmartIBus(uint32_t _baud) :
	SmartIBus(&Serial, _baud)
{
}

SmartIBus::SmartIBus(HardwareSerial* _serial, uint32_t _baud) :
	serial(_serial), baud(_baud)
{
}

SmartIBus::SmartIBus(USBSerial* _serial, uint32_t _baud) :
	usbSerial(_serial), baud(_baud)
{
}

void SmartIBus::begin()
{
  if(serial) serial->begin(baud);
  this->state   = DISCARD;
  this->last    = millis();
  this->ptr     = 0;
  this->len     = 0;
  this->chksum  = 0;
  this->lchksum = 0;
}

void SmartIBus::begin(uint32_t _baud)
{
	baud = _baud;
	this->begin();
}

void SmartIBus::handle(void)
{
	if(!serial) return;
  while (serial->available() > 0)
  {
    uint32_t now = millis();
    if (now - last >= PROTOCOL_TIMEGAP)
    {
      state = GET_LENGTH;
    }
    last = now;
    
    uint8_t v = serial->read();
    switch (state)
    {
      case GET_LENGTH:
        if (v <= PROTOCOL_LENGTH)
        {
          ptr = 0;
          len = v - PROTOCOL_OVERHEAD;
          chksum = 0xFFFF - v;
          state = GET_DATA;
        }
        else
        {
          state = DISCARD;
        }
        break;

      case GET_DATA:
        buffer[ptr++] = v;
        chksum -= v;
        if (ptr == len)
        {
          state = GET_CHKSUML;
        }
        break;
        
      case GET_CHKSUML:
        lchksum = v;
        state = GET_CHKSUMH;
        break;

      case GET_CHKSUMH:
        // Validate checksum
        if (chksum == (v << 8) + lchksum)
        {
          // Execute command - we only know command 0x40
          switch (buffer[0])
          {
            case PROTOCOL_COMMAND40:
              // Valid - extract channel data
              for (uint8_t i = 1; i < IBUS_CHANNELS * 2 + 1; i += 2)
              {
                channel[i / 2] = buffer[i] | (buffer[i + 1] << 8);
              }
              break;

            default:
              break;
          }
        }
        state = DISCARD;
        break;

      case DISCARD:
      default:
        break;
    }
  }
}

uint16_t SmartIBus::read(uint8_t channelNr)
{
  if (channelNr < IBUS_CHANNELS)
  {
    return channel[channelNr];
  }
  else
  {
    return 0;
  }
}
