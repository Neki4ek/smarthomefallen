#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "your_SSID"; // Имя WiFi сети
const char* password = "your_PASSWORD"; // Пароль WiFi сети

const char* serverName = "http://your-server-address.com/relay.php"; // Адрес сервера

void setup() {
  Serial.begin(115200); // Инициализация последовательного порта
  pinMode(D0, OUTPUT); // Назначение пина D0 как выход для управления реле
  WiFi.begin(ssid, password); // Подключение к WiFi сети

  while (WiFi.status() != WL_CONNECTED) { // Ожидание подключения к WiFi
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) { // Проверка подключения к WiFi
    HTTPClient http;
    http.begin(serverName); // Отправка запроса на сервер
    int httpCode = http.GET();

    if (httpCode > 0) { // Обработка ответа сервера
      String payload = http.getString();
      Serial.println(payload);

      if (payload == "1") { // Включение реле
        digitalWrite(D0, HIGH);
      } else if (payload == "0") { // Выключение реле
        digitalWrite(D0, LOW);
      }
    }
    http.end(); // Завершение запроса на сервер
  } else { // Переподключение к WiFi
    WiFi.begin(ssid, password);
    delay(5000);
  }
}
