#include <ELECHOUSE_CC1101_SRC_DRV.h>

// 定义引脚
const int bootButton = 0;
bool sending = false;
bool lastButtonState = HIGH;
byte buffer[256];

uint32_t seed = 0x1c0330c1;  // 32-bit shared seed
uint16_t counter = 0;

String generateRollingCode() {
    char rollingCode[9];
    uint32_t rollingSeed = seed + counter;
    snprintf(rollingCode, sizeof(rollingCode), "%08X", rollingSeed);
    counter++;
    return String(rollingCode);
}

void setup() {
    Serial.begin(9600);

    if (ELECHOUSE_cc1101.getCC1101()) {         // Check the CC1101 Spi connection.
        Serial.println("Connection OK");
    } else {
        Serial.println("Connection Error");
    }

    ELECHOUSE_cc1101.Init();              // must be set to initialize the cc1101!
    ELECHOUSE_cc1101.setCCMode(1);       // set config for internal transmission mode.
    ELECHOUSE_cc1101.setModulation(0);  // set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.
    ELECHOUSE_cc1101.setMHZ(433.92);   // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).
    ELECHOUSE_cc1101.setSyncMode(2);  // Combined sync-word qualifier mode.
    ELECHOUSE_cc1101.setCrc(0);      // CRC calculation in TX and CRC check in RX enabled.

    ELECHOUSE_cc1101.setPA(10);         // Example value, adjust as needed
    ELECHOUSE_cc1101.setDRate(2000);   // Set data rate (adjust as needed)
    pinMode(bootButton, INPUT_PULLUP); // 设置BOOT按键为输入模式
    Serial.println("Tx Mode");
}

void loop() {
    bool currentButtonState = digitalRead(bootButton);
    if (currentButtonState == LOW && lastButtonState == HIGH) {  // 检测BOOT按键的按下和释放
        sending = !sending;
        if (sending) {
            Serial.println("Start sending signal");
        } else {
            Serial.println("Stop sending signal");
        }
        delay(50);  // 防止抖动
    }
    lastButtonState = currentButtonState;

    if (sending) {
        String rollingCode = generateRollingCode();
        rollingCode.toCharArray((char*)buffer, 61);
        ELECHOUSE_cc1101.SendData(buffer, strlen((char*)buffer));
        Serial.println("Sent: " + rollingCode);
        delay(1000);  // 每1秒发送一次消息
    }
}
