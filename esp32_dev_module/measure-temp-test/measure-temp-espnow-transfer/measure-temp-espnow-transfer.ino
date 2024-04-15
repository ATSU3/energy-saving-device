#include <esp_now.h>
#include <WiFi.h>
#include <DHT.h>

// DHTセンサ設定
#define DHTPIN 14       // DHT22のデータピン
#define DHTTYPE DHT22   // DHT22センサ使用
DHT dht(DHTPIN, DHTTYPE);

// 送信先のMACアドレス
uint8_t broadcastAddress[] = {0xC0, 0x49, 0xEF, 0xB5, 0x0E, 0xFC};

// 送信データ構造
struct SensorData {
  float temperature;
  float humidity;
} sensorData;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  dht.begin();

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  
  esp_now_register_send_cb(OnDataSent);
}

void loop() {
  // 温湿度データの読み取り
  sensorData.humidity = dht.readHumidity();
  sensorData.temperature = dht.readTemperature();

  // センサーからの読み取りが失敗した場合のエラーチェック
  if (isnan(sensorData.humidity) || isnan(sensorData.temperature)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    // ESP-NOWでのデータ送信
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &sensorData, sizeof(SensorData));
    if (result == ESP_OK) {
      Serial.print("Sent Temp: ");
      Serial.print(sensorData.temperature);
      Serial.print(" C, Hum: ");
      Serial.print(sensorData.humidity);
      Serial.println("%");
    } else {
      Serial.println("Error sending the data");
    }
  }
  delay(2000);
}
