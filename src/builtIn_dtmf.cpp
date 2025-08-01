#include "Arduino.h"
#include "builtIn_dtmf.h"
int dtmf_class::MAGNITUDE = 1800;
int dtmf_class::adc_div = 512;
uint16_t dtmf_class::sampleNum = 160;
uint16_t dtmf_class::samplingRate = 9828;
////////////////////////////////////////////////////////////////////////
dtmf_class::dtmf_class(byte sensor_pin)
{

  dtmf_sensor_pin = sensor_pin;
  pinMode(dtmf_sensor_pin, INPUT);
};
void dtmf_class::calculateSampleRate()
{
  // callculate the sample rate:
  uint16_t testData[sampleNum];
  uint32_t t2, t1 = micros();
  for (uint16_t index = 0; index < sampleNum; index++)
    testData[index] = analogRead(dtmf_sensor_pin);
  t2 = micros() - t1;
  samplingRate = 1000000 / (t2 / sampleNum);
  // callculate the coeff for each tone:
  float omega;
  for (byte i = 0; i < 8; i++)
  {
    omega = (2.0 * PI * dtmf_tones[i]) / samplingRate;
    coeff[i] = 2.0 * cos(omega);
  }
  resetQ();
}
////////////////////////////////////////////////////////////////////////
void dtmf_class::resetQ(void)
{
  for (byte i = 0; i < 8; i++)
  {
    Q2[i] = 0;
    Q1[i] = 0;
  }
}
///////////////////////////////////////////////////////////////////////
void dtmf_class::ProcessSample(uint16_t sample)
{
  float Q0;
  for (byte i = 0; i < 8; i++)
  {
    Q0 = coeff[i] * Q1[i] - Q2[i] + (float)(sample - adc_div);
    Q2[i] = Q1[i];
    Q1[i] = Q0;
  }
}
/////////////////////////////////////////////////////////////////////
char dtmf_class::button()
{
  int bit = 1;
  int j;
  int dtmf = 0;

  for (byte i = 0; i < 8; i++)
  {
    if (d_mags[i] > MAGNITUDE)
    {
      dtmf |= bit;
    }
    bit <<= 1;
  }
  for (j = 0; j < 16; j++)
  {
    if (dtmf_map[j] == dtmf)
      break;
  }
  if (j < 16)
  {
    // wait for the button to be released
    if (dtmf_char[j] == last_dtmf)
      return ((char)0);
    last_dtmf = dtmf_char[j];
    return (dtmf_char[j]);
  }
  last_dtmf = 0;
  return ((char)0);
}
///////////////////////////////////////////////////////////////////////
char dtmf_class::button_con()
{
  int bit = 1;
  int j;
  int dtmf = 0;
  for (int i = 0; i < 8; i++)
  {
    if (d_mags[i] > MAGNITUDE)
    {
      dtmf |= bit;
    }
    bit <<= 1;
  }
  for (j = 0; j < 16; j++)
  {
    if (dtmf_map[j] == dtmf)
      break;
  }
  if (j < 16)
  {
    return (dtmf_char[j]);
  }
  else
  {
    return (0);
  }
}
////////////////////////////////////////////////////////////////////////
char dtmf_class::read(bool continues)
{
  getMag();
  char char1 = 0;
  if (continues)
    char1 = button();
  else
    char1 = button_con();
  if (char1)
  {
    nochar_count = 0;
    return char1;
  }
  else
  {
    if (++nochar_count > 30000)
      nochar_count = 51;
    return 0;
  }
}
////////////////////////////////////////////////////////////////////////
bool dtmf_class::waitForChar(char ch, unsigned long millisec)
{
  char comming = 0;
  unsigned long startMillis = millis();
  do
  {
    comming = dtmf_class::read();
    if (millisec && (millis() - startMillis) < millisec)
      break;
  } while (comming |= ch);
  return comming;
}
////////////////////////////////////////////////////////////////////////
char dtmf_class::waitForChar(unsigned long millisec)
{
  char comming = 0;
  unsigned long startMillis = millis();
  do
  {
    comming = dtmf_class::read();
    if (millisec && (millis() - startMillis) < millisec)
      break;
  } while (!comming);
  return comming;
}
//////////////////////////////////////////////////////////////////////
int8_t dtmf_class::toNum(char ch)
{
  int8_t a = ch - '0';
  if (a < 0 || a > 9)
    return -1;
  else
    return a;
}
/////////////////////////////////////////////////////////////////////////
int8_t dtmf_class::readInt(bool continues)
{
  return toNum(read(continues));
}
/////////////////////////////////////////////////////////////////////////
int8_t dtmf_class::waitForNum(unsigned long millisec)
{
  int8_t comming = -1;
  unsigned long startMillis = millis();
  do
  {
    comming = dtmf_class::readInt();
    if (millisec && (millis() - startMillis) < millisec)
      break;
  } while (comming < 0);
  return comming;
}
/////////////////////////////////////////////////////////////////////////
void dtmf_class::getMag(bool _debug)
{
  uint32_t t2, t1 = micros();

  // take samples :
  uint16_t testData[sampleNum];
  for (uint16_t index = 0; index < sampleNum; index++)
    testData[index] = analogRead(dtmf_sensor_pin);
  ////////////////////////////////////////////////////
  for (uint16_t index = 0; index < sampleNum; index++)
  {
    ProcessSample(testData[index]);
  }
  float d_tmp;
  for (byte i = 0; i < 8; i++)
  {
    d_tmp = Q1[i] * Q1[i];
    d_tmp += Q2[i] * Q2[i];
    d_tmp -= coeff[i] * Q1[i] * Q2[i];
    d_mags[i] = sqrt(d_tmp);
  }
  resetQ();
    t2 = micros() - t1;
  if (_debug)
  {
    for (uint8_t i = 0; i < 8; i++)
    {
      Serial.print(d_mags[i]);
      Serial.print(" ");
    }
    Serial.println(t2/1000);
  }
}