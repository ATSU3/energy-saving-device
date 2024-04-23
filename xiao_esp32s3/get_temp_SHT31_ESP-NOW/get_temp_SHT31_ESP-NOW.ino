#include <esp_now.h>
#include <WiFi.h>

// 受信データ構造体
struct SensorData {
  float temperature;
  float humidity;
};

// 受信データのコールバック関数
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  SensorData *sensorData = (SensorData *)incomingData;

  Serial.print("Received Temp: ");
  Serial.print(sensorData->temperature);
  Serial.print(" C, Hum: ");
  Serial.print(sensorData->humidity);
  Serial.println(" %");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // ESP-NOWの初期化
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // ESP-NOW受信コールバックの登録
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // このループ内で特にすることはありません。
  delay(1000);  // 次の動作までちょっと待ちます
}
