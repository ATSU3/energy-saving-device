#include <esp_now.h>
#include <WiFi.h>

// 送信側のMACアドレスを指定
// uint8_t senderAddress[] = {0xFC, 0xF5, 0xC4, 0x46, 0xA3, 0x38}; 
uint8_t senderAddress[] = {0x74, 0x4D, 0xBD, 0x81, 0x73, 0xDC}; 

// 受信データを格納する変数
String receivedData;

// 受信コールバック関数
void onDataReceive(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  // 受信データを変換して格納
  receivedData = "";
  for (int i = 0; i < data_len; i++) {
    receivedData += (char)data[i];
  }

  // 受信データを表示
  Serial.print("Received: ");
  Serial.println(receivedData);
}

void setup() {
  // シリアル通信の開始
  Serial.begin(115200);

  // Wi-Fiを無効化（ESP-NOWを使用するため）
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // ESP-NOWの初期化
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // 送信側のペアリング
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, senderAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // ペア情報を追加
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // コールバック関数を登録
  esp_now_register_recv_cb(onDataReceive);
}

void loop() {
  // メインループでは特に処理なし（受信はコールバック関数で処理）
}
