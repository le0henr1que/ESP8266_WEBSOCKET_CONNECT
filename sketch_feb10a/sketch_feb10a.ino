#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "XXXXX";
const char* password = "XXXXXX";
const int ledPin = D7;

WebSocketsServer webSocket(81);
AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  // Configurar LED como saída
  pinMode(ledPin, OUTPUT);

  // Conectar à rede WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando à rede WiFi...");
  }
  Serial.println("Conectado à rede WiFi!");

 server.on("/",
    HTTP_GET, [](AsyncWebServerRequest *request){
      AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", "Servidor WebSocket OK!");
      response->addHeader("Access-Control-Allow-Origin", "*");
      response->addHeader("Access-Control-Allow-Methods", "GET,PUT,POST,DELETE,OPTIONS");
      response->addHeader("Access-Control-Allow-Headers", "*");
      request->send(response);
  });
  server.onNotFound([](AsyncWebServerRequest *request){
    request->send(404);
  });
  server.begin();

  // Iniciar servidor WebSocket
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("Servidor WebSocket iniciado!");
  Serial.print("Endereço IP do servidor: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  webSocket.loop();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_CONNECTED:
      Serial.printf("Cliente conectado: %d\n", num);
      break;
    case WStype_TEXT:
      Serial.printf("Recebido mensagem do cliente %d: %s\n", num, payload);
      if (strcmp((char*)payload, "ligar") == 0) {
        digitalWrite(ledPin, HIGH);
        Serial.println("LED ligado!");
      } else if (strcmp((char*)payload, "desligar") == 0) {
        digitalWrite(ledPin, LOW);
        Serial.println("LED desligado!");
      } else {
        Serial.println("Comando inválido!");
      }
      
      break;
    case WStype_DISCONNECTED:
      Serial.printf("Cliente desconectado: %d\n", num);
      break;
  }
}