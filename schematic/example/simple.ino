#include <Arduino.h>
#include <builtIn_dtmf.h>
#define SPECKER_PIN A2
dtmf_class dtmfReader(SPECKER_PIN);
void setup()
{
  Serial.begin(115200);
  pinMode(A5, OUTPUT); // use pin A5 as gnd for the filter
  digitalWrite(A5, 0);

  pinMode(A4, OUTPUT); // use pin A4 as vcc for the filter
  digitalWrite(A4, 1);
  delay(100); // this delay is neaded for stabalize the filter cause of capacitor.
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // this code is to read filter output voltage .
  // the perfect resault is 512 when R1 = R2 but caus of factory error it is change.
  //  it is better to read this value from serial monitor and write it in the line  // dtmfReader.adc_div = xxx; then comment this code:
 uint32_t avrege = 0;
  for (uint8_t i = 0; i < 250; i++)
  {
    avrege += analogRead(SPECKER_PIN);
  }
  avrege /= 250;
  Serial.println("voltag divider out = : " + String(avrege));
  dtmfReader._adcDiv = (uint16_t)avrege;
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  dtmfReader.magnitudeThreshold = 1000;          // this para is related to volume value (see test_mag.ino)..
}

void loop()
{
  // resault of resd() function is  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'A', 'B', 'C', 'D', '*', '#'
  char dt = dtmfReader.waitForChar();
  if (dt == '1')
  {
    Serial.println("char is '1' ");
  }
  else if (dt == 'A')
  {
    Serial.println("char is 'A' ");
  }
  else if (dt == '*')
  {
    Serial.println("char is '*' ");
  }
  else if (dt > 0)
  {
    Serial.println("char is : " + (String)dt);
  }
}
