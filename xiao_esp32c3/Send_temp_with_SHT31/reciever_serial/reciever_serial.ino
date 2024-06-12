#include <esp_now.h>
#include <WiFi.h>
float temperature;

// データ受信時に実行されるコールバック関数
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&temperature, incomingData, sizeof(temperature));
  Serial.print("Received Temperature: ");
  Serial.println(temperature);
}

void setup() {
  // シリアルモニタを初期化、通信速度115200bps
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA); // デバイスをWi-Fiステーションモードに設定

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
