#include <ELECHOUSE_CC1101_SRC_DRV.h>

uint32_t seed = 0x1c0330c1;  // 32-bit shared seed
uint16_t counter = 0;

String generateRollingCode(uint16_t count) {
    char rollingCode[9];
    uint32_t rollingSeed = seed + count;
    snprintf(rollingCode, sizeof(rollingCode), "%08X", rollingSeed);
    return String(rollingCode);
}

bool isValidRollingCode(String receivedCode) {
    for (uint16_t i = counter; i < counter + 10; i++) { // Check next 10 codes
        if (receivedCode == generateRollingCode(i)) {
            counter = i + 1; // Update counter to next valid code
            return true;
        }
    }
    return false;
}

bool isPreviouslyUsedCode(String receivedCode) {
    for (uint16_t i = 0; i < counter; i++) {
        if (receivedCode == generateRollingCode(i)) {
            return true;
        }
    }
    return false;
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
    ELECHOUSE_cc1101.setModulation(2);  // set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.
    ELECHOUSE_cc1101.setMHZ(433.92);   // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).
    ELECHOUSE_cc1101.setSyncMode(2);  // Combined sync-word qualifier mode.
    ELECHOUSE_cc1101.setCrc(0);      // CRC calculation in TX and CRC check in RX enabled.

    Serial.println("Rx Mode");
}

byte buffer[256] = {0};
int previouslyUsedCodeCount = 0;
String lastUsedCodes[3] = {"", "", ""};

void loop() {
    if (ELECHOUSE_cc1101.CheckRxFifo(100)) {
        if (ELECHOUSE_cc1101.CheckCRC()) {    // CRC Check. If "setCrc(false)" crc returns always OK!
            Serial.print("Rssi: ");
            Serial.println(ELECHOUSE_cc1101.getRssi());
            Serial.print("LQI: ");
            Serial.println(ELECHOUSE_cc1101.getLqi());

            int len = ELECHOUSE_cc1101.ReceiveData(buffer);
            buffer[len] = '\0';
            String receivedCode = String((char*)buffer);
            Serial.println("Received: " + receivedCode);

            if (isValidRollingCode(receivedCode)) {
                Serial.println("Valid code received: " + receivedCode);
                Serial.println("flag{hack_for_fun_creator_by_ch0en3} ");

                previouslyUsedCodeCount = 0; // Reset count if valid code is received
                // Perform action, e.g., unlock door
            } else if (isPreviouslyUsedCode(receivedCode)) {
                lastUsedCodes[previouslyUsedCodeCount % 3] = receivedCode;
                previouslyUsedCodeCount++;
                Serial.println("Previously used code received: " + receivedCode);

                if (previouslyUsedCodeCount >= 3) {
                    Serial.println("Rolling back, valid code: " + lastUsedCodes[(previouslyUsedCodeCount - 1) % 3]);
                    previouslyUsedCodeCount = 0; // Reset the count after rollback
                    Serial.println("flag{hack_for_fun_creator_by_ch0en3} " + receivedCode);

                    // Perform action, e.g., unlock door
                }
            } else {
                Serial.println("Invalid code received: " + receivedCode);
                previouslyUsedCodeCount = 0; // Reset count if invalid code is received
            }
        }
    }
}
