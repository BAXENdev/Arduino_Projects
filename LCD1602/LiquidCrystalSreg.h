
#include <inttypes.h>
#include <Print.h>
#include "ShiftRegister.h"
#include "Arduino.h"

#define RS_COMMAND                      0x00
#define RS_STRING                       0x01

// Sets all characters to space (0x20) and sets cursor to 0x0
#define FNC_CLEARDISPLAY                0x01
// Sets cursor to 0x0 and returns display to 0, if shifted
#define FNC_RETURNHOME                  0x02
// Sets entry mode for characters
#define FNC_ENTRYMODE                   0x04
// Shifts the cursor or display to the right. Display moving right == characters moving left
#define FNC_ENTRYMODE_SHIFTRIGHT        0x02
// Shifts the cursor or display to the right. Display moving right == characters moving left
#define FNC_ENTRYMODE_SHIFTLEFT         0x00
// When shifting, the cursor moves
#define FNC_ENTRYMODE_SHIFTCURSOR       0x00
// When shifting, the display moves
#define FNC_ENTRYMODE_SHIFTDISPLAY      0x01
// Sets the display control
#define FNC_DISPLAYCONTROL              0x08
#define FNC_DISPLAYCONTROL_DISPLAYON    0x04
#define FNC_DISPLAYCONTROL_CURSORON     0x02
#define FNC_DISPLAYCONTROL_BLINKON      0x01
// Sets the LCD display settings to 8-bit mode, 2 lines, 5x8 characters
#define FNC_LCDSETTINGS                 0x38 // 0b00111000

class LiquidCrystalSreg: public Print {
    public:
        LiquidCrystalSreg(uint8_t rs_pin, uint8_t rw_pin, uint8_t enable_pin,
            uint8_t data_pin, uint8_t latch_pin, uint8_t clock_pin);
        void begin(bool displayOn, bool cursorOn, bool cursorBlink);
        void command(uint8_t command);
        virtual size_t write(uint8_t);
        using Print::write; 
    private:
        void send(uint8_t rsValue, uint8_t sregValue);
        ShiftRegister _sreg;
        uint8_t _rs_pin;
        uint8_t _rw_pin;
        uint8_t _enable_pin;
        bool _displayOn = true;
        bool _cursorOn;
        bool _cursorBlink;
};
