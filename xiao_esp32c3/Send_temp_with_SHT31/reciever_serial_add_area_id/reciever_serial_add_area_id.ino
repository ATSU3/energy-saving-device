#include <esp_now.h>
#include <WiFi.h>

// 受信するデータを保持する構造体
struct DataPacket {
  int area_id;
  float temperature;
};

// 受信したデータ
DataPacket receivedData;

// データ受信時に実行されるコールバック関数
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));
  Serial.print("Received Temperature from Area ");
  Serial.print(receivedData.area_id);
  Serial.print(": ");
  Serial.println(receivedData.temperature);
}

void setup() {
  // シリアルモニタを初期化、通信速度115200bps
  Serial.begin(115200);
  
  // デバイスをWi-Fiステーションモードに設定
  WiFi.mode(WIFI_STA);

  // ESP-NOWの初期化
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // データ受信時のコールバック関数を登録
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // メインループでは特に処理を行わない
}
