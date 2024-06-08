
// How long the timer counts down
#define TIMER_VALUE 30
// How long to display "donE." (7 segment letters...)
#define TIMER_DONE 10
// SSEG = Seven Segment
// SREG = Shift Register

/*
A:Top
B:TopRight
C:BottomRight
D:Bottom
E:BottomLeft
F:TopLeft
G:Middle
H:DecimalPoint

Byte structure:
0bHGFEDCBA
*/

#define SSEG_DECIMAL_POINT 0b10000000
#define SSEG_BLANK 0b00000000

/*
This byte structure is read into the shift register, where each bit corresponds to
    an output pin. Each letter refers to the output pin of the shift register when the
    byte is outputed.
Byte structure:
0bHGFEDCBA

This is not the most optimal structure when it comes to wires. I designed this to match
    the letters of the register pins to the registers of the segment display.
    If you are OCD about wires, you may want to redesign this. Bytes are written major bit first.
*/
uint8_t ssegDigits[] = {
//  0bHGFEDCBA
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};

uint8_t ssegLetters[] = {
//  0bHGFEDCBA
    0b01110111, // A
    0b01111100, // b
    0b00111001, // C
    0b01011110, // d
    0b01111001, // E
    0b01110001, // F
    0b01111101, // G (Same as 6)
    0b01110110, // H
    0b00110000, // I (Same as 1, but on left side)
    0b00011110, // J
    SSEG_BLANK, // K (Blank)
    0b00111000, // L
    SSEG_BLANK, // M (Blank)
    0b01010100, // n
    0b01011100, // o
    0b01110011, // P
    0b01100111, // q (Similar to 9)
    0b01010000, // r
    0b01101101, // S (Same as 5)
    0b01111000, // t
    0b00111110, // U
    0b00011100, // v (Looks like lower case U)
    SSEG_BLANK, // W (Blank)
    SSEG_BLANK, // X (Blank)
    0b01101110, // y
    0b01011011  // Z (Same as 2)
};

#define SSEG_PIN_DIGIT_1 7
#define SSEG_PIN_DIGIT_2 5
#define SSEG_PIN_DIGIT_3 3
#define SSEG_PIN_DIGIT_4 1

//
#define SREG_PIN_CLOCK 10
#define SREG_PIN_LATCH 11
#define SREG_PIN_DATA 12

uint8_t ssegDisplayDigits[4] = {SSEG_BLANK, SSEG_BLANK, SSEG_BLANK, SSEG_BLANK};
uint8_t ssegDecimalPoints[4] = {SSEG_BLANK, SSEG_BLANK, SSEG_BLANK, SSEG_BLANK};
uint8_t ssegDigitPins[4] = {SSEG_PIN_DIGIT_1, SSEG_PIN_DIGIT_2, SSEG_PIN_DIGIT_3, SSEG_PIN_DIGIT_4};

void setup()
{
	pinMode(SREG_PIN_LATCH, OUTPUT);
	pinMode(SREG_PIN_CLOCK, OUTPUT);
	pinMode(SREG_PIN_DATA, OUTPUT);

    digitalWrite(SREG_PIN_LATCH, LOW);
    digitalWrite(SREG_PIN_CLOCK, LOW);
    digitalWrite(SREG_PIN_DATA, LOW);

    pinMode(SSEG_PIN_DIGIT_1, OUTPUT);
    pinMode(SSEG_PIN_DIGIT_2, OUTPUT);
    pinMode(SSEG_PIN_DIGIT_3, OUTPUT);
    pinMode(SSEG_PIN_DIGIT_4, OUTPUT);

    digitalWrite(SSEG_PIN_DIGIT_1, LOW);
    digitalWrite(SSEG_PIN_DIGIT_2, HIGH);
    digitalWrite(SSEG_PIN_DIGIT_3, HIGH);
    digitalWrite(SSEG_PIN_DIGIT_4, HIGH);
};
long startTime = millis() / 1000; // in seconds
void loop() {
    long timeLeft = TIMER_VALUE - ((millis() / 1000) - startTime);

    if (timeLeft <= 0) {
        ssegDisplayDigits[0] = ssegLetters['D' - 65];
        ssegDisplayDigits[1] = ssegLetters['O' - 65];
        ssegDisplayDigits[2] = ssegLetters['N' - 65];
        ssegDisplayDigits[3] = ssegLetters['E' - 65] | SSEG_DECIMAL_POINT;
        long doneTime = millis() / 1000;
        while ((millis() / 1000) - doneTime < TIMER_DONE) {
            ssegDisplayUpdate();
        };
        startTime = millis() / 1000; // reset start time
    };

    ssegDisplayDigits[0] = ssegDigits[timeLeft / 1000 % 10]; // EX: 14605 / 1000 % 10 = 4
    ssegDisplayDigits[1] = ssegDigits[timeLeft / 100 % 10]; // EX: 14605 / 100 % 10 = 6
    ssegDisplayDigits[2] = ssegDigits[timeLeft / 10 % 10]; // EX: 14605 / 10 % 10 = 0
    ssegDisplayDigits[3] = ssegDigits[timeLeft % 10]; // EX: 14605 % 10 = 5

    ssegDisplayUpdate();
};

void sregWrite(uint8_t value) {
    // Disable output
    digitalWrite(SREG_PIN_LATCH, LOW);
    // Write value to shift register
    for (int i = 0; i < 8; i++) {
        // Turn off the clock
        digitalWrite(SREG_PIN_CLOCK, LOW);
        // Write next bit to shift register, major bit first
        digitalWrite(SREG_PIN_DATA, value & 0b10000000);
        value = value << 1;
        digitalWrite(SREG_PIN_CLOCK, HIGH);
    };
    // Enable output
    digitalWrite(SREG_PIN_LATCH, HIGH);
    // TODO: Should I always turn clock back off since output is locked?
};

uint8_t lastDigit = SSEG_PIN_DIGIT_1;
void ssegDisplayUpdate() {
    for (int i = 0; i < 4; i++) {
        // Turn off current digit to prevent ghosting
        digitalWrite(lastDigit, HIGH);
        // write new digit to shift register -> display
        sregWrite(ssegDisplayDigits[i] | ssegDecimalPoints[i]);
        // Turn on next digit and update last digit to current digit
        digitalWrite(ssegDigitPins[i], LOW);
        lastDigit = ssegDigitPins[i];
        // Delay to allow display to show evenly
        delay(1);
        /*
        delay here required to keep the difference in average delay between
        each sregWrite high enough so the perceived brightness is maintained.
        If no delay, the delay between writing digit 4->1 and digit 1->2, 2->3,
        and 3->4 is so different that brightness for digits 1-3 is low and
        digit 4's brightness is high. WOW!
        */
    };
};