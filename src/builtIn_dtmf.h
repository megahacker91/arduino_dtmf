


#ifndef builtIn_dtmf_h
#define builtIn_dtmf_h

#include "Arduino.h"

class dtmf_class
{

public:
    static int MAGNITUDE;
    static int adc_div ;
    static uint16_t sampleNum ;
    static uint16_t samplingRate;

    int8_t dtmf_sensor_pin = -1;
    int8_t dtmf_led_pin = -1;

    dtmf_class(byte sensor_pin);
void calculateSampleRate();
    char read(bool continues=false);
    char waitForChar(unsigned long millisec =0);
    bool waitForChar(char ch, unsigned long millisec=0);

    int8_t readInt(bool continues=false);
    int8_t waitForNum(unsigned long millisec);
    int8_t waitForNum(byte num ,unsigned long millisec);
    void getMag(bool _debug=false);
private:
    char button();
    char button_con();
    void ProcessSample(uint16_t);
    void resetQ(void);
    int8_t toNum(char);
    float coeff[8];
    float Q1[8];
    float Q2[8];
    u32 nochar_count = 0;
    float d_mags[8];
    char last_dtmf = 0;

    const int dtmf_tones[8] = {697, 770, 852, 941, 1209, 1336, 1477, 1633};
    const unsigned char dtmf_map[16] = {0x11, 0x21, 0x41, 0x12, 0x22, 0x42, 0x14, 0x24, 0x44, 0x28, 0x81, 0x82, 0x84, 0x88, 0x18, 0x48};
    const char dtmf_char[16] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'A', 'B', 'C', 'D', '*', '#'};
};

#endif
