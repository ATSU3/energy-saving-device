#include <esp_now.h>
#include <WiFi.h>
#define BAUDRATE 9600

typedef struct struct_message {
    float temp;
    float hum;
} struct_message;

struct_message myData;

void setup() {
  Serial.begin(BAUDRATE);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  // シリアル通信を介してカンマ区切り形式でデータをArduinoに送信
  Serial.print(myData.temp);
  Serial.print(",");
  Serial.println(myData.hum);
}

void loop() {
  delay(1000);
}
