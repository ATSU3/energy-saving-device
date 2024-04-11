void setup() {
  Serial.begin(115200);
}

void loop() {
  if (Serial.available()) {
    String receivedData = Serial.readStringUntil('\n');
    Serial.print("受信データ: ");
    Serial.println(receivedData);
  }
}
