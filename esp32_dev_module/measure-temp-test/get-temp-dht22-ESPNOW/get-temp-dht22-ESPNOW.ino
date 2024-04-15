#include <WiFi.h>
#include <esp_now.h>

// 受信データ構造体
struct SensorData {
  float temperature;
  float humidity;
};

// 送信側のMACアドレス
uint8_t broadcastAddress[] = {0x7C, 0x9E, 0xBD, 0x93, 0x2A, 0x84};

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  SensorData *sensorData = (SensorData *)incomingData;
  Serial.print("Received temperature: ");
  Serial.print(sensorData->temperature);
  Serial.print(" C, Humidity: ");
  Serial.print(sensorData->humidity);
  Serial.println(" %");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // ESP-NOWの初期化
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // 送信側のペア設定
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0; // 同じチャンネルに設定してください
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // 受信コールバック関数の登録
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  delay(1000);  
}
