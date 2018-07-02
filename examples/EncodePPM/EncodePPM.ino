#include <SmartPPM.h>

#define PPM_PIN           3
#define PPM_CHANNELS      6

SmartPPM ppm(PPM_PIN, PPM_CHANNELS);

void handlePPM_INT() {
  ppm.handlePPM();
}

void setup()
{
    Serial.begin(9600);
    attachInterrupt(PPM_PIN, handlePPM_INT, RISING);
}

void loop()
{
    // Print latest valid values from all channels
    for (int channel = 0; channel < PPM_CHANNELS; ++channel) {
        unsigned long value = ppm.getValid(channel);
        Serial.print(String(value) + " ");
    }
    Serial.println();
}

