#include <Arduino.h>
#include <Wire.h>
#include <AE_SHT31.h>

// SHT31のアドレスを設定
AE_SHT31 SHT31 = AE_SHT31(0x45); // Open


void setup() {
  Serial.begin(115200);
  // SHT31を使用するためのI2Cピンを明示的に設定
  Wire.begin(5, 6); 
  // Wire.begin(6, 7); ESP32C3の場合
  // Wire.begin(5, 6); ESP32S3の場合
  // シリアルに文字を出力
  Serial.println("SHT31 Test!!");
  // SHT31をソフトリセット
  SHT31.SoftReset();
  // 内蔵ヒーター 0:OFF 1:ON
  SHT31.Heater(0);
}



void loop()
{
  // SHT31から温湿度データを取得
  SHT31.GetTempHum();
  
  Serial.println("--------------------------");
  // SHT31.Temperature()　より温度データ取得
  Serial.println("Temperature ('C)");
  Serial.println(SHT31.Temperature());
  // SHT31.Humidity()　より相対湿度データ取得
  Serial.println("Humidity (%)");
  Serial.println(SHT31.Humidity());
  // 待ち時間
  delay(10000);
}