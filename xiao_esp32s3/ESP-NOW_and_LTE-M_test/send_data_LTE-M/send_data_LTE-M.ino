#define TINY_GSM_MODEM_BG96
#include <TinyGsmClient.h>
#include <SoftwareSerial.h>

#define RX 0
#define TX 1
#define BAUDRATE 9600
SoftwareSerial mySerial(RX, TX);

TinyGsm modem(mySerial);

const char apn[] = "soracom.io";
const char user[] = "sora";
const char pass[] = "sora";
const char server[] = "harvest.soracom.io";
const int port = 8514;

void setup() {
  mySerial.begin(BAUDRATE);
  Serial.begin(BAUDRATE);

  if (!modem.restart()) {
    Serial.println("Failed to restart modem");
  } else if (!modem.gprsConnect(apn, user, pass)) {
    Serial.println("Failed to connect");
  } else {
    Serial.println("Connected to cellular network");
  }
}

void loop() {
  if (mySerial.available()) {
    String data = mySerial.readStringUntil('\n');
    Serial.println("Received data: " + data);  // 受信データを表示
    int commaIndex = data.indexOf(',');
    if (commaIndex > 0) {
      String tempStr = data.substring(0, commaIndex);
      String humStr = data.substring(commaIndex + 1);
      float temp = tempStr.toFloat();
      float hum = humStr.toFloat();
      Serial.print("Parsed Temperature: ");
      Serial.print(temp);
      Serial.print(" C, Parsed Humidity: ");
      Serial.println(hum);
      
      if (modem.isGprsConnected()) {
        TinyGsmClient client(modem);
        if (client.connect(server, port)) {
          client.print(String("{\"temperature\":") + temp + ",\"humidity\":" + hum + "}");
          client.stop();
          Serial.println("Data sent to Soracom Harvest");
        } else {
          Serial.println("Failed to connect to server");
        }
      }
    }
  }
  delay(5000);
}
