#include <DHT.h>
#include <esp_now.h>
#include <WiFi.h>

#define DHTPIN 20
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

typedef struct struct_message {
    float temp;
    float hum;
} struct_message;

struct_message myData;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Data Send Status: ");
  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("Success");
  } else {
    Serial.println("Failure");
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  // ここでESP32デバイス1のMACアドレスを追加
  uint8_t broadcastAddress[] = {0xEC, 0xDA, 0x3B, 0xBB, 0xDB, 0xB8};

  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);
}

void loop() {
  myData.temp = dht.readTemperature();
  myData.hum = dht.readHumidity();

  esp_now_send(0, (uint8_t *) &myData, sizeof(myData));
  delay(10000);
}
