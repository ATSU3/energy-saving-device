#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"

// SHT31センサーのインスタンスを作成
Adafruit_SHT31 sht31 = Adafruit_SHT31();

// 送信先のMACアドレス
// uint8_t broadcastAddress[] = {0xEC, 0xDA, 0x3B, 0xBB, 0xB1, 0xE4};
uint8_t broadcastAddress[] = {0xEC, 0xDA, 0x3B, 0xBB, 0xDB, 0xB8};

// ピア情報を保持する構造体
esp_now_peer_info_t peerInfo;

// 送信するデータを保持する構造体
struct DataPacket {
  int area_id;
  float temperature;
};

// 送信するデータ
DataPacket dataToSend;

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
  
  // デバイスをWi-Fiステーションモードに設定
  WiFi.mode(WIFI_STA);

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

  // area_idを設定
  dataToSend.area_id = 4;
}

void loop() {
  // SHT31センサーにシングルショットモードの測定コマンドを送信
  Wire.beginTransmission(0x45); // SHT31のI2Cアドレスに送信を開始
  Wire.write(0x24);             // シングルショット測定のコマンドMSB
  Wire.write(0x00);             // シングルショット測定のコマンドLSB
  Wire.endTransmission();

  delay(15); // 測定完了までの待機時間（通常は15ms）

  // 測定結果を読み取る
  Wire.requestFrom(0x45, 6); // 6バイトのデータをリクエスト（温度2バイト + CRC1バイト + 湿度2バイト + CRC1バイト）

  if (Wire.available() == 6) {
    uint16_t tempData = Wire.read() << 8 | Wire.read(); // 温度データ
    Wire.read(); // CRCバイトを無視
    // uint16_t humidityData = Wire.read() << 8 | Wire.read(); // 湿度データ
    // Wire.read(); // CRCバイトを無視

    // 温度データを計算
    dataToSend.temperature = -45.0 + 175.0 * ((float)tempData / 65535.0);

    Serial.print("Temperature: ");
    Serial.println(dataToSend.temperature);
    
    // 温度データとエリアIDを送信
    esp_now_send(broadcastAddress, (uint8_t *)&dataToSend, sizeof(dataToSend));
  } else {
    Serial.println("Failed to read temperature");
  }

  delay(5000); // 5秒間隔でデータを送信
}
