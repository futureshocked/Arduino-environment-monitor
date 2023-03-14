#include <dhtnew.h>

#define DHT11_PIN 11
DHTNEW mySensor(DHT11_PIN); 

void setup()
{
  Serial.begin(9600);
  Serial.println("DHT TEST PROGRAM ");
  Serial.print("Sensor type: DHT");
  Serial.println(mySensor.getType());
  Serial.println("Humidity (%),\tTemperature (C)");
}

void loop()
{
  mySensor.read();
  Serial.print(mySensor.getHumidity(), 1);
  Serial.print("\t");
  Serial.println(mySensor.getTemperature(), 1);
  delay(5000);
}