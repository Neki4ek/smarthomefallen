#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Настройки Wi-Fi
const char* ssid = "Ваш_сетевой_имя";
const char* password = "Ваш_пароль";

// Порт веб-сервера
const int webServerPort = 80;

// Пин, к которому подключено реле
const int relayPin = D1;

// Создаем экземпляр веб-сервера
ESP8266WebServer server(webServerPort);

// Функция для включения реле
void turnOnRelay() {
  digitalWrite(relayPin, HIGH);
}

// Функция для выключения реле
void turnOffRelay() {
  digitalWrite(relayPin, LOW);
}

// Обработчик HTTP-запросов для включения реле
void handleTurnOn() {
  turnOnRelay();
  server.send(200, "text/plain", "Relay turned on");
}

// Обработчик HTTP-запросов для выключения реле
void handleTurnOff() {
  turnOffRelay();
  server.send(200, "text/plain", "Relay turned off");
}

// Обработчик HTTP-запросов для получения состояния реле
void handleGetStatus() {
  String status = digitalRead(relayPin) == HIGH ? "on" : "off";
  server.send(200, "text/plain", status);
}

// Отправка файла index.html
void handleRoot() {
  String html = "<html>"
                "<head><script src='https://ajax.googleapis.com/ajax/libs/jquery/3.6.0/jquery.min.js'></script></head>"
                "<body>"
                "<h1>Управление реле</h1>"
                "<button id='onButton'>Включить</button>"
                "<button id='offButton'>Выключить</button>"
                "<p id='statusText'></p>"
                "<script>"
                "function updateStatus() {"
                "  $.ajax({"
                "    url: '/status',"
                "    type: 'GET',"
                "    success: function(data) {"
                "      $('#statusText').text('Состояние реле: ' + data);"
                "    }"
                "  });"
                "}"
                "$(document).ready(function() {"
                "  $('#onButton').click(function() {"
                "    $.ajax('/on');"
                "    updateStatus();"
                "  });"
                "  $('#offButton').click(function() {"
                "    $.ajax('/off');"
                "    updateStatus();"
                "  });"
                "  setInterval(updateStatus, 1000);"
                "});"
                "</script>"
                "</body>"
                "</html>";
  server.send(200, "text/html", html);
}

void setup() {
  // Инициализация пина реле
  pinMode(relayPin, OUTPUT);
  turnOffRelay();

  // Подключение к Wi-Fi сети
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Подключено к сети: ");
  Serial.println(ssid);
  Serial.print("IP-адрес: ");
  Serial.println(WiFi.localIP());
   // Настройка обработчиков HTTP-запросов
  server.on("/", handleRoot);
  server.on("/on", handleTurnOn);
  server.on("/off", handleTurnOff);
  server.on("/status", handleGetStatus);

  // Запуск веб-сервера
  server.begin();
  Serial.println("Веб-сервер запущен");
}

void loop() {
  // Обработка HTTP-запросов
  server.handleClient();
}