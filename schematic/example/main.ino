#include <Arduino.h>
#include <builtIn_dtmf.h>

#define SAFE_TIME 3 // in seconds

#define HIGH_SIDE_OUTPUT_PIN 9
#define LOW_SIDE_OUTPUT_PIN 8
#define SPECKER_PIN A2
dtmf_class dtmfReader(SPECKER_PIN);

void setup()
{
  Serial.begin(115200);
  pinMode(HIGH_SIDE_OUTPUT_PIN, OUTPUT);
  digitalWrite(HIGH_SIDE_OUTPUT_PIN, 1);
  pinMode(LOW_SIDE_OUTPUT_PIN, OUTPUT);
  digitalWrite(LOW_SIDE_OUTPUT_PIN, 0);
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
  dtmfReader.magnitudeThreshold = 1000; // this para is related to volume value (see test_mag.ino)..
  // dtmfReader.dtmf_led_pin = LED_BUILTIN; // optinal led to blink when detect dtmf code
  uint32_t safeTimer = millis() + (SAFE_TIME * 1000);
  while (safeTimer > millis())
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(900);
  }
}

void loop()
{
  char c1 = dtmfReader.waitForChar(0ul);
  Serial.println("c1 :" + String(c1));
  char c2 = dtmfReader.waitForChar(0, 5000UL);
  if (c2)
    Serial.println("c2 :" + String(c2));
  else
    Serial.println("no char detected");
  if (c1 == '1' && c2 == '5')
  {
    Serial.println("fire");
    digitalWrite(HIGH_SIDE_OUTPUT_PIN, 0);
    digitalWrite(LOW_SIDE_OUTPUT_PIN, 1);
    delay(2000);
    digitalWrite(HIGH_SIDE_OUTPUT_PIN, 1);
    digitalWrite(LOW_SIDE_OUTPUT_PIN, 0);
  }
}
