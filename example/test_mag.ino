//https://onlinetonegenerator.com/ you can use this site to generate dtmf signals to test your circuit
// connect aux cable to your phone or pc 3.5 mm jack then connect the other side to your filter input
//then upload test_mag.ino file to arduino then open serial ploter and try all dtmf tones then select the best magnitude 
// (the best threshold that all pressed tones above it and others never arrive it )
#include <Arduino.h>
#include <builtIn_dtmf.h>
#define SPECKER_PIN A5
dtmf_class dtmfReader(SPECKER_PIN);
void setup()
{
  Serial.begin(115200);
  pinMode(A5, OUTPUT); //use pin A5 as gnd for the filter 
  digitalWrite(A5, 0);

  pinMode(A4, OUTPUT); //use pin A4 as vcc for the filter 
  digitalWrite(A4, 1);
  delay(100); // this delay is neaded for stabalize the filter cause of capacitor.
  uint32_t avrege = 0;
  for (uint8_t i = 0; i < 250; i++)
  {
    avrege += analogRead(SPECKER_PIN);
  }
  avrege /= 250;
  Serial.println("voltag divider out = : " + String(avrege));
  dtmfReader.adc_div = avrege;
  dtmfReader.calculateSampleRate();
  Serial.println("sample rate is = : " + String(dtmfReader.samplingRate));
}

void loop()
{
  dtmfReader.getMag(true);
}
