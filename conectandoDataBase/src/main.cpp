#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup()
{
  Serial.begin(115200);
  Serial.println("Hello World!");
}

void loop()
{
  Serial.println("ESP32 funcionando!");
  delay(5000);
}