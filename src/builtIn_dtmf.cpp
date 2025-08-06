#include "Arduino.h"
#include "builtIn_dtmf.h"

dtmf_class::dtmf_class(byte sensorPin)
{
  _sensorPin = sensorPin;
  _sampleRate = 9828.0;
  _sampleNum = 140;
  _adcDiv = 512;
  magnitudeThreshold = 1500.0;
  ResetQ();
}
void dtmf_class::ResetQ(void)
{
  for (int i = 0; i < 8; i++)
  {
    Q2[i] = 0;
    Q1[i] = 0;
  }
}

void dtmf_class::ProcessSample(uint16_t sample)
{
  float Q0;
  for (int i = 0; i < 8; i++)
  {
    Q0 = coeff[i] * Q1[i] - Q2[i] + (float)((int)((int)sample - (int)_adcDiv));
    Q2[i] = Q1[i];
    Q1[i] = Q0;
  }
}
void dtmf_class::getMags(bool debug)
{
  uint16_t sampleData[_sampleNum];
  float d_tmp=0;
  for (int index = 0; index < _sampleNum; index++)
  {
    sampleData[index] = analogRead(_sensorPin);
  }
  for (int index = 0; index < _sampleNum; index++)
  {
    ProcessSample(sampleData[index]);
  }

  for (int i = 0; i < 8; i++)
  {
    // This is the equivalent of sqrt(real*real + imag*imag)
    d_tmp = Q1[i] * Q1[i];
    d_tmp += Q2[i] * Q2[i];
    d_tmp -= coeff[i] * Q1[i] * Q2[i];
    dtmf_mag[i] = sqrt(d_tmp);
  }
  ResetQ();
  if (debug)
  {
    for (uint8_t i = 0; i < 8; i++)
    {
      Serial.print(dtmf_mag[i]);
      Serial.print(" ");
    }
    Serial.println();
  }
}
char dtmf_class::read(bool con)
{
  getMags();
  int j;
  int dtmf = 0;
  for (int i = 0; i < 8; i++)
  {
    if (dtmf_mag[i] > magnitudeThreshold)
    {
      dtmf |= bit(i);
    }
  }
  for (j = 0; j < 16; j++)
  {
    if (dtmf_map[j] == dtmf)
      break;
  }
  if (j < 16)
  {
    if (dtmf_char[j] == last_dtmf && !con)
      return ((char)0);
    last_dtmf = dtmf_char[j];
    return (dtmf_char[j]);
  }
  last_dtmf = 0;
  return ((char)0);
}

char dtmf_class::waitForChar(char ch=0, unsigned long millisec = 0)
{
  uint32_t startMillis = millis();
  char thisChar=0;
  do
  {
    thisChar = read();
    if(ch>0 && thisChar == ch) break;
    else if(thisChar >0)break;
    if(millisec>0 && (millis()-startMillis >= millisec) ) break;
  } while (true);
  return thisChar;
}

