#include <Arduino.h>
#include <WiFi.h>

// Configurações do Wi-Fi
const char *ssid = "Tigre";
const char *password = "amandaline";

void setup()
{
  Serial.begin(115200);

  // Conectar ao Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado ao Wi-Fi!");
}

void loop()
{
  int valor = random(0, 100);

  Serial.print("Mostrando Valor: ");
  Serial.println(valor);

  delay(5000);
}