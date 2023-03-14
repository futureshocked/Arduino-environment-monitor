// In this variation of the sketch, the only change is to move the DHTNEW mySensor 
// constructor to the sensorTimer function. This way, the RAM that is tied up 
// by the mySensor object is released outside the function and avoids "breaking"
// the operation of the sketch.

// Comment this out to disable prints and save space
//#define BLYNK_PRINT Serial

#include "credentials.h"
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <dhtnew.h>
#include <LiquidCrystal.h>

#define DHT11_PIN 4
//DHTNEW mySensor(DHT11_PIN); // If the mySensor object is created here,
                              // the Arduino Uno will run out of SRAM and
                              // the program will not work.
                              // Create the mySensor object in the sensorTimer
                              // library where it is actually needed.

LiquidCrystal lcd(13, 12, 8, 9, 10, 11);

#define PHOTOR_PIN A0

char auth[] = BLYNK_AUTH_TOKEN;

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);

#define ledPin 5

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

  DHTNEW mySensor(DHT11_PIN); 
  mySensor.read();
  int tem = mySensor.getTemperature();
  int hum = mySensor.getHumidity();

  Blynk.virtualWrite(V2, tem);
  Blynk.virtualWrite(V3, hum);

  updateLCD(tem, hum, lux);
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0,1);
  lcd.print(F("Connecting..."));

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

void updateLCD(int tem, int hum, int lux)
{
  lcd.clear();
  lcd.setCursor(0,0); //Set the cursor XY coordinates
  lcd.print(F("TEM:"));
  lcd.print(tem, DEC);

  lcd.setCursor(0,1); //Set the cursor XY coordinates
  lcd.print(F("HUM:"));
  lcd.print(hum, DEC);

  lcd.setCursor(7,0); //Set the cursor XY coordinates
  lcd.print(F("LUX:"));
  lcd.print(lux, DEC);
}