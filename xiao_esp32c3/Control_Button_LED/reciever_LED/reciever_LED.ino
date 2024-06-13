#include <esp_now.h>
#include <WiFi.h>    

const int lightME = 10; // LEDを接続するピン番号を定義

int lightsOn; // 受信したデータを格納するための変数

// データを受信したときに実行されるコールバック関数
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  // 受信データをlightsOn変数にコピー
  memcpy(&lightsOn, incomingData, sizeof(lightsOn));
  // シリアルモニタに受信したデータを表示
  Serial.print("button: ");
  Serial.println(lightsOn);
  // LEDの状態を受信したデータに基づいて変更
  digitalWrite(lightME, lightsOn);
}
 
void setup() {
  // シリアルモニタを初期化、通信速度115200bps
  Serial.begin(115200);
  // LEDピンを出力モードに設定
  pinMode(lightME, OUTPUT);
  
  // デバイスをWi-Fiステーションモードに設定
  WiFi.mode(WIFI_STA);

  // ESP-NOWの初期化
  if (esp_now_init() != ESP_OK) {
    // 初期化に失敗した場合、エラーメッセージを表示
    Serial.println("Error initializing ESP-NOW");
    return; // 初期化失敗時は関数を終了
  }
  
  // ESP-NOWの受信コールバック関数を登録
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  // ループは特に何もしない（全ての処理はコールバック関数で行う）
}
