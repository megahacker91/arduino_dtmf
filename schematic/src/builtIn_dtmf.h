#ifndef DTMF_h
#define DTMF_h

// include types & constants of Wiring core API
#include "Arduino.h"

class dtmf_class
{
public:
    dtmf_class(byte);
    char read(bool con = 0);
    char waitForChar(char ch = 0, unsigned long millisec = 0);
    void getMags(bool debug = 0);
    uint16_t _sampleNum;
    float _sampleRate;
    float magnitudeThreshold;
    uint16_t _adcDiv;
    void dtmf_class::calculateSampleRate()
    {
        uint32_t t2, t1 = micros();
        for (uint16_t index = 0; index < _sampleNum; index++)
            analogRead(_sensorPin);
        t2 = micros() - t1;
        _sampleRate = 1000000 / (t2 / _sampleNum);
    }

private:
    const unsigned char dtmf_map[16] = {0x11, 0x21, 0x41, 0x12, 0x22, 0x42, 0x14, 0x24, 0x44, 0x28, 0x81, 0x82, 0x84, 0x88, 0x18, 0x48};
    const char dtmf_char[16] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'A', 'B', 'C', 'D', '*', '#'};
    const float coeff[8] = {1.76, 1.71, 1.65, 1.58, 1.32, 1.18, 1.01, 0.82};
    byte _sensorPin;
    char last_dtmf;
    float dtmf_mag[8];
    float Q1[8];
    float Q2[8];
    void ProcessSample(uint16_t);
    void ResetQ(void);
};

#endif
