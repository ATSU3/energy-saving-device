#include "WiFi.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("WiFi MAC Address:");
  Serial.println(WiFi.macAddress());
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("WiFi MAC Address:");
  Serial.println(WiFi.macAddress());
  delay(5000);
}
