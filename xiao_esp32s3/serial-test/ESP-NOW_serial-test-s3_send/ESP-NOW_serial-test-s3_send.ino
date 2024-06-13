#include <esp_now.h>
#include <WiFi.h>

// 受信側のMACアドレスを指定
// uint8_t receiverAddress[] = {0x7C, 0x9E, 0xBD, 0x93, 0x2A, 0x84}; 
uint8_t receiverAddress[] = {0x74, 0x4D, 0xBD, 0x81, 0x69, 0x30}; 
// 送信データの固定長を設定（例として最大32バイトに設定）
#define DATA_LENGTH 32
uint8_t dataToSend[DATA_LENGTH] = {0};

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

  // 受信側のペアリング
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // ペア情報を追加
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  if (Serial.available() > 0) {
    // シリアル通信からデータを読み取る
    String data = Serial.readStringUntil('\n');
    
    // データを送信データ配列にコピー
    memset(dataToSend, 0, DATA_LENGTH); // 配列をクリア
    strncpy((char *)dataToSend, data.c_str(), DATA_LENGTH - 1); // データをコピー

    // データを送信
    esp_err_t result = esp_now_send(receiverAddress, dataToSend, DATA_LENGTH);

    if (result == ESP_OK) {
      Serial.println("Message sent successfully");
    } else {
      Serial.println("Error sending the message");
    }
  }
}
