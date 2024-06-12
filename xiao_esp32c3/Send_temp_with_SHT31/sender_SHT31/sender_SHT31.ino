#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"

Adafruit_SHT31 sht31 = Adafruit_SHT31();
uint8_t broadcastAddress[] = {0xEC, 0xDA, 0x3B, 0xBB, 0xB1, 0xE4};
esp_now_peer_info_t peerInfo;
float temperature;

// データ送信後に実行されるコールバック関数
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // シリアルモニタを初期化、通信速度115200bps
  Serial.begin(115200);
  
  // SHT31センサーを初期化
  if (!sht31.begin(0x45)) {
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
  
  WiFi.mode(WIFI_STA); // デバイスをWi-Fiステーションモードに設定

  // ESP-NOWの初期化
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
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
    return;
  }
}

void loop() {
  // SHT31センサーから温度を取得
  temperature = sht31.readTemperature();
  if (!isnan(temperature)) {  // 温度が正常に読み取れた場合
    Serial.print("Temperature: ");
    Serial.println(temperature);
    
    // 温度データを送信
    esp_now_send(broadcastAddress, (uint8_t *)&temperature, sizeof(temperature));
  } else {
    Serial.println("Failed to read temperature");
  }
  delay(1000); // 1秒間隔でデータを送信
}
