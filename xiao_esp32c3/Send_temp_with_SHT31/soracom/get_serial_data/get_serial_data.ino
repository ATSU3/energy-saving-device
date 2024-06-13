void setup() {
  // シリアルモニタを初期化、通信速度115200bps
  Serial.begin(115200);
}

void loop() {
  // ESP32からのデータがある場合
  if (Serial.available() > 0) {
    // データを読み取る
    String data = Serial.readStringUntil('\n');
    
    // カンマで分割
    int commaIndex = data.indexOf(',');
    if (commaIndex > 0) {
      int area_id = data.substring(0, commaIndex).toInt();
      float temperature = data.substring(commaIndex + 1).toFloat();
      
      // シリアルモニタに表示
      Serial.print("Received from ESP32 - Area ID: ");
      Serial.print(area_id);
      Serial.print(", Temperature: ");
      Serial.println(temperature);
    }
  }
}
