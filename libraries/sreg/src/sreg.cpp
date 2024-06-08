
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

SREG::SREG(uint8_t data_pin, uint8_t clock_pin, uint8_t latch_pin) {
    _data_pin = data_pin;
    _clock_pin = clock_pin;
    _latch_pin = latch_pin;

    pinMode(_data_pin, OUTPUT);
    pinMode(_clock_pin, OUTPUT);
    pinMode(_latch_pin, OUTPUT);
}

void SREG::write(uint8_t value) {
    digitalWrite(_latch_pin, LOW);
    for (int i = 0; i < 8; i++) {
        digitalWrite(_clock_pin, LOW);
        // Write next bit to shift register, major bit first
        digitalWrite(_data_pin, value & 0b10000000);
        value = value << 1;
        digitalWrite(_clock_pin, HIGH);
    };
    digitalWrite(_latch_pin, HIGH);
}
