
#include "LiquidCrystalSreg.h"

#define RS_PIN 7
#define RW_PIN 5
#define ENABLE_PIN 3

#define DATA_PIN 13
#define LATCH_PIN 11
#define CLOCK_PIN 9


// Smiley characters 0-2, 4-6
byte customCharacters[8][8] = {
    // Character 1
    {
        B00000,
        B00000,
        B00011,
        B00100,
        B01001,
        B01011,
        B10001,
        B10000
    },
    // Character 2
    {
        B00000,
        B11111,
        B00000,
        B00000,
        B00000,
        B10001,
        B00000,
        B00000
    },
    // Character 3
    {
        B00000,
        B00000,
        B11000,
        B00100,
        B10010,
        B11010,
        B10001,
        B00001
    },
    // Character 4
    {
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000
    },
    // Character 5
    {
        B10000,
        B10001,
        B01001,
        B01000,
        B00100,
        B00011,
        B00000,
        B00000
    },
    // Character 6
    {
        B00000,
        B00000,
        B00000,
        B10001,
        B01110,
        B00000,
        B11111,
        B00000
    },
    // Character 7
    {
        B00001,
        B10001,
        B10010,
        B00010,
        B00100,
        B11000,
        B00000,
        B00000
    },
    // Character 8
    {
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000
    }
};
LiquidCrystalSreg lcd(RS_PIN, RW_PIN, ENABLE_PIN, DATA_PIN, LATCH_PIN, CLOCK_PIN);

void setup () {
    Serial.begin(115200);
    Serial.setTimeout(10000);

    lcd.print("Hello World!");
};

uint8_t buffer[] = {0x00, 0x00};
void loop () {
    size_t numBytes = Serial.readBytes(buffer,2);
    if (numBytes < 2) {
    } else {
        if (buffer[0] == 0x00) {
            lcd.command(buffer[1]);
        } else {
            lcd.print((char) buffer[1]);
        };
    };
    delay(500);
};
