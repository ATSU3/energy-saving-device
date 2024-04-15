#include <Wire.h>
#include <Adafruit_SHT31.h>

Adafruit_SHT31 sht31 = Adafruit_SHT31();

void setup() {
    Serial.begin(115200);
    if (!sht31.begin(0x44)) {   // SHT31のデフォルトI2Cアドレスは0x44
        Serial.println("Couldn't find SHT31");
        while (1) delay(1);
    }
}

void loop() {
    float temp = sht31.readTemperature();
    float hum = sht31.readHumidity();

    if (!isnan(temp)) {  // 温度が正しく読み取られた場合
        Serial.print("Temp *C = ");
        Serial.println(temp);
    } else {
        Serial.println("Failed to read temperature");
    }

    if (!isnan(hum)) {  // 湿度が正しく読み取られた場合
        Serial.print("Hum. % = ");
        Serial.println(hum);
    } else {
        Serial.println("Failed to read humidity");
    }

    delay(1000);
}
