
// PIN Variables
// Either wire the components to the corresponding pin
// Or redefine the values to match the pins
#define EAST_LED 5
#define WEST_LED 10
#define SOUTH_LED 6
#define NORTH_LED 11
#define X_PIN 14 // A0
#define Y_PIN 15 // A1
#define CLICKED_PIN 2

// Constants
#define LED_BRIGHTNESS 64
#define DELAY 200
#define JOYSTICK_ZERO 512
#define JOYSTICK_THRESHOLD 96

/**
 * @brief Set all LED pins to output. 
 * 
 */
void setup()
{
    // Output PINS

    pinMode(WEST_LED, OUTPUT);
    pinMode(EAST_LED, OUTPUT);
    pinMode(SOUTH_LED, OUTPUT);
    pinMode(NORTH_LED, OUTPUT);

    // Input PINS

    pinMode(CLICKED_PIN, INPUT_PULLUP);
    pinMode(X_PIN, INPUT);
    pinMode(Y_PIN, INPUT);

    // Setup

    dimLed(EAST_LED);
    dimLed(WEST_LED);
    dimLed(NORTH_LED);
    dimLed(SOUTH_LED);
}

/**
 * @brief Read Joystick.
 * If the joystick is pressed, light all LEDs.
 * Read the X & Y input, determine direction, and light up corresponding LEDs.
 * 
 */
void loop()
{
    // Read input

    int x = analogRead(X_PIN);
    int y = analogRead(Y_PIN);
    
    // If joystick is pressed, light all LEDs.
    // If joystick is not pressed, input is 1.

    if (!digitalRead(CLICKED_PIN))
    {
        lightLed(NORTH_LED);
        lightLed(WEST_LED);
        lightLed(SOUTH_LED);
        lightLed(EAST_LED);
        return;
    }

    //EAST
    if (x - JOYSTICK_THRESHOLD > JOYSTICK_ZERO)
    {
        dimLed(WEST_LED);
        lightLed(EAST_LED);
    }
    //WEST
    else if (x + JOYSTICK_THRESHOLD < JOYSTICK_ZERO)
    {
        dimLed(EAST_LED);
        lightLed(WEST_LED);
    }
    //"CENTERED"
    else 
    {
        dimLed(WEST_LED);
        dimLed(EAST_LED);
    }

    // The Joystick's y-axis is inverted.
    // NORTH
    if (y + JOYSTICK_THRESHOLD < JOYSTICK_ZERO)
    {
        dimLed(SOUTH_LED);
        lightLed(NORTH_LED);
    }
    // SOUTH
    else if (y - JOYSTICK_THRESHOLD > JOYSTICK_ZERO)
    {
        dimLed(NORTH_LED);
        lightLed(SOUTH_LED);
    }
    //"CENTERED"
    else 
    {
        dimLed(NORTH_LED);
        dimLed(SOUTH_LED);
    }
}

void dimLed(uint8_t led)
{
    analogWrite(led, 0);
}

void lightLed(uint8_t led)
{
    analogWrite(led, LED_BRIGHTNESS);
}

