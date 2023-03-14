// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include "credentials.h"
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);

const int ledPin=5;
const int potPin=A0;

BlynkTimer timer;  // We'll use the BlynkTimer to take
                    // pot readings every 1000 ms.
                    // This will protect the Blynk cloud from 
                    // flooding. 
                    // Read more: https://docs.blynk.io/en/blynk.edgent-firmware-api/blynk-timer

BLYNK_WRITE(V0)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V0 to a variable
  digitalWrite(ledPin,pinValue);
}

void potTimer() 
{
  // This function describes what will happen with each timer tick
  // e.g. writing sensor value to datastream V1
  int potValue = analogRead(potPin);
  Blynk.virtualWrite(V1, potValue);
  Serial.println(potValue); 
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  Blynk.begin(auth, wifi, ssid, pass);
  pinMode(ledPin,OUTPUT);

  // Setting interval to send data to Blynk Cloud to 1000ms. 
  // It means that data will be sent every second
  timer.setInterval(1000L, potTimer);
}

void loop()
{
  Blynk.run();
  timer.run(); 
}