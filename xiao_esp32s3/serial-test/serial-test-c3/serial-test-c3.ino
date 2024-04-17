#include <HardwareSerial.h>

HardwareSerial mySerial(1); // UART1を使用

void setup() {
  // GPIO16をRX, GPIO17をTXとして設定
  mySerial.begin(115200, SERIAL_8N1, 20, 21);
}

void loop() {
  mySerial.println("Hello, Arduino!");
  delay(1000);
}
