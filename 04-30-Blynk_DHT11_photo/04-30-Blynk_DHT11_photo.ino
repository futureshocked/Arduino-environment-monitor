// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include "credentials.h"
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <dhtnew.h>

#define DHT11_PIN 4
DHTNEW mySensor(DHT11_PIN); 

#define PHOTOR_PIN A0

char auth[] = BLYNK_AUTH_TOKEN;

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);

const int ledPin=5;

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

void sensorTimer() 
{
  // This function describes what will happen with each timer tick
  // e.g. writing sensor values to datastream V1, V2, V3.
  int photoRValue = analogRead(PHOTOR_PIN);
  int lux = 1.68 * photoRValue - 833;
  Blynk.virtualWrite(V1, lux);

  mySensor.read();
  Blynk.virtualWrite(V2, mySensor.getTemperature());
  Blynk.virtualWrite(V3, mySensor.getHumidity());
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  
  Serial.print("Sensor type: DHT");
  Serial.println(mySensor.getType());
  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  Blynk.begin(auth, wifi, ssid, pass);
  pinMode(ledPin,OUTPUT);

  // Setting interval to send data to Blynk Cloud to 1000ms. 
  // It means that data will be sent every second
  timer.setInterval(5000L, sensorTimer);
}

void loop()
{
  Blynk.run();
  timer.run(); 
}