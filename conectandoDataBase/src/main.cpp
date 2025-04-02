#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <HTTPClient.h> // Adicionado para HTTP_CODE_UNAUTHORIZED

// Configurações do WiFi
#define WIFI_SSID "Tigre"
#define WIFI_PASSWORD "amandaline"

// Configurações do Firebase
#define FIREBASE_HOST "esp32database-ab229-default-rtdb.firebaseio.com/" // Sem https://
#define FIREBASE_AUTH "6YPpyBsCbd4F293QB3lNVUeq0CxDMhjCz44bYWib"         // Encontre em Configurações do projeto > Contas de serviço > Segredos do banco de dados

// Estrutura para armazenar dados dos sensores
struct SensorData
{
  float temperatura1;
  float temperatura2;
  float temperatura3;
  float umidade;
  bool fogoDetectado;
  unsigned long timestamp;
};

// Declaração antecipada das funções (prototypes)
void conectarWiFi();
void configurarFirebase();
SensorData lerSensores();
void enviarDadosFirebase(SensorData dados);
void imprimirDadosSerial(SensorData dados);

// Objetos Firebase
FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth;

void setup()
{
  Serial.begin(115200);

  conectarWiFi();
  configurarFirebase();
}

void loop()
{
  static unsigned long ultimoEnvio = 0;

  if (millis() - ultimoEnvio >= 1000)
  {
    ultimoEnvio = millis();

    SensorData dados = lerSensores();
    enviarDadosFirebase(dados);
    imprimirDadosSerial(dados);
  }
}

void conectarWiFi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Conectado com IP: ");
  Serial.println(WiFi.localIP());
}

void configurarFirebase()
{
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Firebase.setReadTimeout(fbdo, 1000 * 15);
  Firebase.setwriteSizeLimit(fbdo, "tiny");
}

SensorData lerSensores()
{
  SensorData dados;

  // Simulação - substitua por leituras reais
  dados.temperatura1 = random(200, 350) / 10.0;
  dados.temperatura2 = random(180, 320) / 10.0;
  dados.temperatura3 = random(190, 330) / 10.0;
  dados.umidade = random(300, 800) / 10.0;
  dados.fogoDetectado = random(100) < 5;
  dados.timestamp = millis();

  return dados;
}

void enviarDadosFirebase(SensorData dados)
{
  FirebaseJson json;

  json.set("temperatura/ponto1", dados.temperatura1);
  json.set("temperatura/ponto2", dados.temperatura2);
  json.set("temperatura/ponto3", dados.temperatura3);
  json.set("umidade", dados.umidade);
  json.set("alerta/fogo", dados.fogoDetectado);
  json.set("timestamp", dados.timestamp);

  String caminho = "/sensor_data/" + String(dados.timestamp);

  if (Firebase.setJSON(fbdo, caminho, json))
  {
    Serial.println("Dados enviados com sucesso!");
  }
  else
  {
    Serial.println("Falha ao enviar dados:");
    Serial.println(fbdo.errorReason());

    if (fbdo.httpCode() == HTTP_CODE_UNAUTHORIZED)
    {
      Serial.println("Reconectando ao Firebase...");
      Firebase.begin(&config, &auth);
    }
  }
}

void imprimirDadosSerial(SensorData dados)
{
  Serial.println("----- Dados dos Sensores -----");
  Serial.print("Temperatura 1: ");
  Serial.print(dados.temperatura1);
  Serial.println(" °C");
  Serial.print("Temperatura 2: ");
  Serial.print(dados.temperatura2);
  Serial.println(" °C");
  Serial.print("Temperatura 3: ");
  Serial.print(dados.temperatura3);
  Serial.println(" °C");
  Serial.print("Umidade: ");
  Serial.print(dados.umidade);
  Serial.println(" %");
  Serial.print("Fogo detectado: ");
  Serial.println(dados.fogoDetectado ? "SIM" : "NÃO");
  Serial.print("Timestamp: ");
  Serial.println(dados.timestamp);
  Serial.println("-----------------------------");
}