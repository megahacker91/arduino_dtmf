#include <Arduino.h>
#include <builtIn_dtmf.h>
#define SPECKER_PIN A5
dtmf_class dtmfReader(SPECKER_PIN);
void setup()
{
  Serial.begin(115200);
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // this code is to read filter output voltage .
  // the perfect resault is 512 when R1 = R2 but caus of factory error it is change.
  //  it is better to read this value from serial monitor and write it in the line  // dtmfReader.adc_div = xxx; then comment this code:
  uint32_t avrege = 0;
  for (size_t i = 0; i < 128; i++)
  {
    avrege += analogRead(SPECKER_PIN);
  }
  avrege /= 128;
  Serial.println("voltag divider out = : " + String(avrege));
  dtmfReader.adc_div = avrege;
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  dtmfReader.dtmf_led_pin = LED_BUILTIN; // optinal led to blink when detect dtmf code
  dtmfReader.MAGNITUDE = 1800;           // this para is related to volume value (see test_mag.ino)..
}

void loop()
{
  // resault of resd() function is  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'A', 'B', 'C', 'D', '*', '#'
  char dt = dtmfReader.read();
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
    Serial.println("char is : " + dt);
  }
  delay(200);
}
