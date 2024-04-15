#include "DHT.h"

#define DHTPIN 14  // DHT22のデータピンをGPIO 15に接続
#define DHTTYPE DHT22   // DHT22を使用
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // センサーからの読み取り間隔は最小2秒必要
  delay(2000);

  float humidity = dht.readHumidity();  // 湿度を読む
  float temperature = dht.readTemperature();  // 温度を読む（摂氏）

  // センサーからの読み取りが成功したかチェック
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // シリアルモニタに温度と湿度を出力
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");
}
