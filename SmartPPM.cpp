#include "SmartPPM.h"

SmartPPM::SmartPPM(uint8_t pin, uint8_t channels)
{
    if (channels) {
        // Setup an array for storing channel values
        this->channelAmount = channels;
        rawValues   = new unsigned long[channels];
        validValues = new unsigned long[channels];
        for (int i = 0; i < channels; ++i) {
            rawValues[i]   = 0;
            validValues[i] = 0;
        }
        // Attach an interrupt to the pin
        this->interruptPin = pin;
        pinMode(interruptPin, INPUT);
    }
}

SmartPPM::~SmartPPM()
{
    delete [] rawValues;
    delete [] validValues;
	this->channelAmount = 0;
}

void SmartPPM::handlePPM()
{
    // Remember the current micros() and calculate the time since the last pulseReceived()
    unsigned long previousMicros = microsAtLastPulse;
    microsAtLastPulse = micros();
    unsigned long time = microsAtLastPulse - previousMicros;

    if (time > PPM_BLANK_TIME)
	{
        /* If the time between pulses was long enough to be considered an end
         * of a signal frame, prepare to read channel values from the next pulses */
        pulseCounter = 0;
    }
    else
	{
        // Store times between pulses as channel values
        if (pulseCounter < channelAmount) {
            rawValues[pulseCounter] = time;
            if (time >= PPM_MIN - PPM_MAX_ERROR && time <= PPM_MAX + PPM_MAX_ERROR) {
                validValues[pulseCounter] = constrain(time, PPM_MIN, PPM_MAX);
            }
        }
        ++pulseCounter;
    }
}

uint16_t SmartPPM::getRaw(uint8_t channel)
{
    unsigned long value = 0;
    if (channel < channelAmount) {
        noInterrupts();
        value = rawValues[channel];
        interrupts();
    }
    return value;
}

uint16_t SmartPPM::getValid(uint8_t channel) {
    unsigned long value = 0;
    if (channel < channelAmount) {
        noInterrupts();
        value = validValues[channel];
        interrupts();
    }
    return value;
}
