#define TINY_GSM_MODEM_BG96
#include <TinyGsmClient.h>
#include <SoftwareSerial.h>

#define RX 0
#define TX 1
#define BAUDRATE 9600
SoftwareSerial mySerial(RX, TX);


TinyGsm modem(mySerial);

const char apn[]      = "soracom.io";
const char user[]     = "sora";
const char pass[]     = "sora";
const char server[]   = "harvest.soracom.io";
const int  port       = 8514;

void setup() {
  mySerial.begin(115200);
  Serial.begin(115200);

  modem.restart();
  modem.gprsConnect(apn, user, pass);

  Serial.println("Connected to cellular network");
}

void loop() {
  if (Serial.available()) {
    float temp = Serial.parseFloat();
    float hum = Serial.parseFloat();
    Serial.println("Sending data to Soracom Harvest...");

    if (modem.isGprsConnected()) {
      TinyGsmClient client(modem);
      if (client.connect(server, port)) {
        client.print(String("{\"temperature\":") + temp + ",\"humidity\":" + hum + "}");
        client.stop();
        Serial.println("Data sent");
      }
    }
  }
  delay(5000);
}
