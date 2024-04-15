#include <Wire.h>
#include <DHT.h>
#include <esp_now.h>
#include <WiFi.h>

#define DHTPIN 14         // DHT22センサが接続されているピンの番号
#define DHTTYPE DHT22    // 使用するDHTモデル

DHT dht(DHTPIN, DHTTYPE);

// 送信先のMACアドレス
uint8_t broadcastAddress[] = {0xC0, 0x49, 0xEF, 0xB5, 0x0E, 0xFC};

// 送信データ構造体
struct SensorData {
  float temperature;
  float humidity;
} sensorData;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Data send status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // ESP-NOWの初期化
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // 送信先のペア設定
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // ESP-NOW送信コールバックの登録
  esp_now_register_send_cb(OnDataSent);

  // DHT22の初期化
  dht.begin();
}

void loop() {
  sensorData.humidity = dht.readHumidity();
  sensorData.temperature = dht.readTemperature();

  // 温度と湿度のデータが有効か確認
  if (!isnan(sensorData.temperature) && !isnan(sensorData.humidity)) {
    Serial.print("Sending Temp: ");
    Serial.print(sensorData.temperature);
    Serial.print(" C, Hum: ");
    Serial.print(sensorData.humidity);
    Serial.println(" %");

    // データの送信
    esp_now_send(broadcastAddress, (uint8_t *) &sensorData, sizeof(SensorData));
  } else {
    Serial.println("Failed to read from sensor");
  }

  delay(2000);  // 2秒ごとに測定と送信
}
