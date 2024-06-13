#include <esp_now.h>
#include <WiFi.h>

const int pushDown = 10; // ボタンが接続されるピン番号を定義
int oldSwitchState = 0;  // 前回のボタンの状態を保持する変数
int lightsOn = 0;        // LEDのオン/オフ状態を保持する変数

// 受信機のMACアドレスを定義
uint8_t broadcastAddress[] = {0xEC, 0xDA, 0x3B, 0xBB, 0xCF, 0x54};

int state; // 現在のボタンの状態を保持する変数
esp_now_peer_info_t peerInfo; // ピア情報を保持する構造体

// データ送信後に実行されるコールバック関数
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  // 送信ステータスをシリアルモニタに表示
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  pinMode(pushDown, INPUT); // ボタンピンを入力モードに設定
  Serial.begin(115200); // シリアルモニタを初期化、通信速度115200bps
 
  WiFi.mode(WIFI_STA); // デバイスをWi-Fiステーションモードに設定

  // ESP-NOWの初期化
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return; // 初期化失敗時は関数を終了
  }

  // データ送信後のコールバック関数を登録
  esp_now_register_send_cb(OnDataSent);
  
  // ピア情報を設定
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // ピアを追加
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return; // 追加に失敗した場合は関数を終了
  }
}
 
void loop() {
  state = digitalRead(pushDown); // ボタンの現在の状態を読み取る
  if (state != oldSwitchState) { // ボタンの状態が変わったかどうかをチェック
    oldSwitchState = state; // 状態が変わった場合、前回の状態を更新
    if (state == HIGH) { // ボタンが押された場合
      lightsOn = !lightsOn; // LEDのオン/オフ状態をトグル
    }
  }
  Serial.println(lightsOn); // 現在のLEDの状態をシリアルモニタに表示

  // データを送信
  esp_now_send(broadcastAddress, (uint8_t *)&lightsOn, sizeof(lightsOn));

  delay(30); // 30ミリ秒の遅延
}