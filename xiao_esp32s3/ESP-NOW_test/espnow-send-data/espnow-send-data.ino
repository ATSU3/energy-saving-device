
#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0x74, 0x4D, 0xBD, 0x81, 0x6D, 0x08};
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_peer_info_t peerInfo;
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
  int randomNumber = random(0, 100);
  Serial.print("Sending: ");
  Serial.println(randomNumber);
  esp_now_send(broadcastAddress, (uint8_t *)&randomNumber, sizeof(randomNumber));
  delay(2000);
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("Send Success");
  } else {
    Serial.println("Send Fail");
  }
}
