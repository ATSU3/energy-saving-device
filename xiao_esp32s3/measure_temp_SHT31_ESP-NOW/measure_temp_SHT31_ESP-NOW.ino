#include <Arduino.h>
#include <Wire.h>
#include <AE_SHT31.h>
#include <esp_now.h>
#include <WiFi.h>

// SHT31のアドレスを設定
AE_SHT31 SHT31 = AE_SHT31(0x45);

// 送信先のMACアドレス（実際のMACアドレスを設定してください）
uint8_t broadcastAddress[] = {0x74, 0x4D, 0xBD, 0x81, 0x69, 0x30};

// 送信データ構造体
struct SensorData {
  float temperature;
  float humidity;
} sensorData;

// ESP-NOW送信コールバック
void OnDataSent(const uint8_t *mac, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("Success");
  } else {
    Serial.println("Failure");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("SHT31 ESP-NOW Sender");

  // ESP32S3のI2Cピン設定
  Wire.begin(5, 6);

  // Wi-FiをSTAモードで初期化
  WiFi.mode(WIFI_STA);

  // ESP-NOW初期化
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // 送信コールバックを登録
  esp_now_register_send_cb(OnDataSent);

  // 送信先ピアを追加
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
  return;
}
}

void loop() {
  // SHT31から温湿度データを取得
  SHT31.GetTempHum();
  sensorData.temperature = SHT31.Temperature();
  sensorData.humidity = SHT31.Humidity();

  Serial.println("--------------------------");
  Serial.println("Temperature ('C)");
  Serial.println(sensorData.temperature);
  Serial.println("Humidity (%)");
  Serial.println(sensorData.humidity);

  // センサーデータをESP-NOWで送信
  esp_now_send(broadcastAddress, (uint8_t *)&sensorData, sizeof(sensorData));
  delay(10000); // 送信間隔を10秒に設定
}
