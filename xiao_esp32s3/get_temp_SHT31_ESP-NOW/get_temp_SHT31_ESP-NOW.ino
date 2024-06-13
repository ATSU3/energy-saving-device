#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// 受信データ構造体
struct SensorData {
  float temperature;
  float humidity;
};

// 送信元のMACアドレスを設定（実際のMACアドレスに置き換えてください）
uint8_t broadcastAddress[] = {0x74, 0x4D, 0xBD, 0x81, 0x73, 0xDC};

// ESP-NOW受信コールバック
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
  if (len != sizeof(SensorData)) {
    Serial.println("Invalid data length");
    return;
  }

  SensorData *data = (SensorData *)incomingData;
  Serial.print("Received Temperature: ");
  Serial.print(data->temperature);
  Serial.println(" °C");
  Serial.print("Received Humidity: ");
  Serial.print(data->humidity);
  Serial.println(" %");
}

void setup() {
  Serial.begin(115200);
  Serial.println("SHT31 ESP-NOW Receiver");

  // Wi-FiをSTAモードで初期化
  WiFi.mode(WIFI_STA);

  // ESP-NOWの初期化
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // 送信元のピアを追加
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  // チャンネル0は現在のWi-Fiチャンネルを使用
  peerInfo.encrypt = false;  // データの暗号化は行わない
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // ESP-NOW受信コールバックを登録
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // ここでは特に処理は行いません
}
