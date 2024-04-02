#include <esp_now.h>
#include <WiFi.h>

uint8_t LED[2];            // 送信データ(LEDのON=1,OFF=0）
char buf[3];               // 受信データバッファを1バイト増やして、NULL終端文字用のスペースを確保

// LED用PIN
const int LED_PIN = 12;

// 受信データ完了した時の処理
void OnDataRecv(const uint8_t *mac, const uint8_t *recvData, int len) {
  if (len > 0) {
    // バッファサイズを超えないようにデータの長さを調整
    int copyLen = len < sizeof(buf) - 1 ? len : sizeof(buf) - 1;
    memcpy(buf, recvData, copyLen);
    buf[copyLen] = '\0';          // 文字列の最後にNULL終端文字を設定

    Serial.print("buf : ");
    Serial.println(buf);

    if(atoi(buf) == 1) {
      digitalWrite(LED_PIN, HIGH);   // LED ON
    }
    else {
      digitalWrite(LED_PIN, LOW);    // LED OFF
    }
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);      // Wi-FiをStationモードに設定

  // ESP-NOWの初期化
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);  // ESP-NOWでデータ受信した時のコールバック関数を登録
  pinMode(LED_PIN, OUTPUT);               // LED_PINを出力に設定
}

void loop() {

}
