
#include "LiquidCrystalSreg.h"

// #include <stdio.h>
// #include <string.h>
// #include <inttypes.h>

LiquidCrystalSreg::LiquidCrystalSreg(uint8_t rs_pin, uint8_t rw_pin, uint8_t enable_pin,
    uint8_t data_pin, uint8_t latch_pin, uint8_t clock_pin) {
    
    _rs_pin = rs_pin;
    _rw_pin = rw_pin;
    _enable_pin = enable_pin;

    _sreg = ShiftRegister(data_pin, latch_pin, clock_pin);

    pinMode(_rs_pin, OUTPUT);
    pinMode(_rw_pin, OUTPUT);
    pinMode(_enable_pin, OUTPUT);

    digitalWrite(_rs_pin, LOW);
    digitalWrite(_rw_pin, LOW);
    digitalWrite(_enable_pin, LOW);

    begin(true, true, true);
};

// As opposed to the original Arduino LiquidCrystal library, this library
// assumes use with the LCD1602 in 8-bit mode with 2-line display.
// begin() takes data on setting display functions instead.
void LiquidCrystalSreg::begin(bool displayOn, bool cursorOn, bool cursorBlink) {
    // Set lcd to 8-bit mode, 2 lines, and 5x8 characters
    delay(200);
    command(FNC_LCDSETTINGS);

    _displayOn = displayOn;
    _cursorOn = cursorOn;
    _cursorBlink = cursorBlink;

    uint8_t displayControl = FNC_DISPLAYCONTROL;
    if (_displayOn) {
        displayControl |= FNC_DISPLAYCONTROL_DISPLAYON;
    };
    if (_cursorOn) {
        displayControl |= FNC_DISPLAYCONTROL_CURSORON;
    };
    if (_cursorBlink) {
        displayControl |= FNC_DISPLAYCONTROL_BLINKON;
    };
    command(displayControl);
    command(FNC_CLEARDISPLAY);
    command(FNC_RETURNHOME);
};

void LiquidCrystalSreg::command(uint8_t sregValue) {
    send(RS_COMMAND, sregValue);
};

size_t LiquidCrystalSreg::write(uint8_t sregValue) {
    send(RS_STRING, sregValue);
    return 1; // assume sucess
};

void LiquidCrystalSreg::send(uint8_t rsValue, uint8_t sregValue) {
    if (Serial) {
        Serial.print("Printed hex value: ");
        Serial.println(rsValue, HEX);
    };
    digitalWrite(_rs_pin, rsValue);
    _sreg.write(sregValue);
    delayMicroseconds(1); // delay between RS and Enable pin
    digitalWrite(_enable_pin, HIGH);
    delayMicroseconds(100); // delay for data to be read
    digitalWrite(_enable_pin, LOW);
};
