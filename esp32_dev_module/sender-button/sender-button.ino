#include <esp_now.h>
#include <WiFi.h>

// 送信先のMACアドレス
uint8_t broadcastAddress[] = {0xC0, 0x49, 0xEF, 0xB5, 0x0E, 0xFC};

const int tact_SW = 12; // ボタンが接続されているピンの番号

uint8_t LED[2];           
char buf[2];
int temp = 0;
int tact_data;          
boolean flag = false;   

esp_now_peer_info_t peerInfo;   

// 送信が完了した時の処理
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Deli_Success" : "Deli_Fail");
}
 
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // 12番ピンを入力として設定し、内部プルアップ抵抗を有効にする
  pinMode(tact_SW, INPUT_PULLUP);

  // ESP-NOWの初期化
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // ペア情報（受信側のアドレスとチャネルと暗号化の設定）
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // 受信側の情報追加      
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  esp_now_register_send_cb(OnDataSent);   // ESP-NOWでデータ送信した時のコールバック関数を登録
}
 
void loop() {
  tact_data = digitalRead(tact_SW);    // 12番ピンの状態を読み取る
  if(tact_data == LOW){    // ボタンが押された時
    flag = !flag; // フラグをトグル
    temp = flag ? 1 : 0; // LEDの状態をセット
    sprintf(buf, "%d", temp); // 数値を文字列に変換
    memcpy(LED, buf, sizeof(buf)); // LED変数に値をコピー
    // ESP-NOWでのデータ送信
    esp_err_t result = esp_now_send(broadcastAddress, LED, sizeof(LED));
   
    if (result == ESP_OK) {
      Serial.println("Success");
      Serial.print("Temp: ");
      Serial.println(temp);
    } else {
      Serial.println("Error in transmission");
    }
    delay(500);
  }
}
