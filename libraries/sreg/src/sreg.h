

#include <inttypes.h>

class SREG{

    public:
        SREG(uint8_t data_pin, uint8_t clock_pin, uint8_t latch_pin);
        void write(uint8_t value);
    private:
        uint8_t _data_pin;
        uint8_t _clock_pin;
        uint8_t _latch_pin;
};
