#include <esp_now.h>
#include <WiFi.h>

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {

}

// データ受信コールバック関数
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  int receivedData = 0;
  memcpy(&receivedData, incomingData, sizeof(receivedData));
  Serial.print("Received: ");
  Serial.println(receivedData);
}
