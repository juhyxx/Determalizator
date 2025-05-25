#include <Arduino.h>
#include <avr/sleep.h>

#define PIN_LED 3    // PB3 (pin 2)
#define PIN_JUMPER 4 // PB4 (pin 3)
#define PIN_SERVO 0  // PB0 (pin 5)
#define PIN_DEBUG 1  // PB1 (pin  6)
#define PIN_START 2  // PB2 (pin 7)

const int initTime[] = {120, 300}; // čas v sekundách pro timeout
const int timeQ = 500;
long time = initTime[0];

volatile bool buttonOn = false;

ISR(PCINT0_vect)
{
    buttonOn = true;
}
void sendServoPulse(int angle)
{
    int pulseWidth = map(angle, 0, 180, 500, 2500); // μs
    digitalWrite(PIN_SERVO, HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(PIN_SERVO, LOW);
}

void moveServoTo(int angle)
{
    for (int i = 0; i < 25; i++)
    {
        sendServoPulse(angle);
        delay(20);
    }
}

void kick()
{
    moveServoTo(180);
}

void reset()
{
    moveServoTo(0);
}

void setup()
{
    pinMode(PIN_START, INPUT_PULLUP);
    pinMode(PIN_JUMPER, INPUT_PULLUP);
    pinMode(PIN_DEBUG, INPUT_PULLUP);
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_SERVO, OUTPUT);

    // Povolit přerušení na pin change
    GIMSK |= (1 << PCIE);
    PCMSK |= (1 << PCINT2); // PB2
    sei();                  // globální povolení přerušení
    reset();                // výchozí pozice
}

void sleepNow()
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    cli();
    sleep_bod_disable();
    sei();
    sleep_cpu();
    sleep_disable();
}

void loop()
{
    if (buttonOn)
    {
        buttonOn = false;
        if (!digitalRead(PIN_DEBUG))
        {
            time = 10;
        }
        else
        {
            time = digitalRead(PIN_JUMPER) ? initTime[0] : initTime[1];
        }

        digitalWrite(PIN_LED, HIGH);
        long duration = (long)time * 1000;
        for (long t = duration; t > 0; t -= timeQ)
        {
            delay(timeQ);
            if (buttonOn)
            {
                buttonOn = false;
                t = time * 1000; // reset časovače
            }
        }
        kick();
        delay(1000);
        reset();
        delay(1000);
    }
    digitalWrite(PIN_LED, LOW);
    sleepNow();
}
