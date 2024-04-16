#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message {
    float temp;
    float hum;
} struct_message;

struct_message myData;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Received - Temp: ");
  Serial.print(myData.temp);
  Serial.print(" Humidity: ");
  Serial.println(myData.hum);
}

void loop() {
  delay(1000);
}
