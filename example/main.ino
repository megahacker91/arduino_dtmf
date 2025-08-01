#include <Arduino.h>
#include <builtIn_dtmf.h>

#define SAFE_TIME 10 // in seconds

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
  // use arduino pins as vcc and gnd for filter resistors:
  pinMode(A5, OUTPUT);
  digitalWrite(A5, 0);

  pinMode(A4, OUTPUT);
  digitalWrite(A4, 1);
  delay(100); // this delay to make sure that filter capacitor is fully charged.
              // get the output of filter voltage divider
  uint32_t avrege = 0;
  for (uint8_t i = 0; i < 250; i++)
  {
    avrege += analogRead(SPECKER_PIN);
  }
  avrege /= 250;
  Serial.println("voltag divider out = : " + String(avrege));
  dtmfReader.adc_div = avrege;
  // get sample rate of adc
  dtmfReader.calculateSampleRate();
  Serial.println("sample rate is = : " + String(dtmfReader.samplingRate));
  // dtmfReader.MAGNITUDE = xxx;//xxx from // this para is related to volume value (see test_mag.ino)..

  dtmfReader.dtmf_led_pin = LED_BUILTIN; // optinal led to blink when detect dtmf code
  uint32_t safeTimer = millis() + (SAFE_TIME * 1000);
  while (millis() > safeTimer)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(900);
  }
}

void loop()
{
  char c1 = dtmfReader.waitForChar();
  char c2 = dtmfReader.waitForChar(2000UL);
  if (c1 == '1' && c2 == '5')
  {
    digitalWrite(HIGH_SIDE_OUTPUT_PIN, 0);
    digitalWrite(LOW_SIDE_OUTPUT_PIN, 1);
    delay(2000);
    digitalWrite(HIGH_SIDE_OUTPUT_PIN, 1);
    digitalWrite(LOW_SIDE_OUTPUT_PIN, 0);
  }
}
