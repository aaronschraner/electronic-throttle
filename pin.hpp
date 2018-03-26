#ifndef PIN_H
#define PIN_H

#include <avr/io.h> // _BV

#include <stdint.h>

#include "register.hpp"

enum Direction {
    INPUT,
    OUTPUT
};

enum PinValue: bool {
    HIGH = true,
    LOW = false
};

struct Pin {
    reg8_t port;
    uint8_t pin;

    Pin(reg8_t port, uint8_t pin, Direction d = INPUT) :
        port(port), pin(pin)
    {
        mode(d);
        set(0);
    }

    void mode(Direction const d) {
        if (d == OUTPUT)
            ddr_reg() |= _BV(pin);
        else
            ddr_reg() &= ~_BV(pin);
    }

    void set(bool const value) {
        port = value ? 
            port | _BV(pin) :
            port &~_BV(pin);
    }
    
    bool get() const {
        return pin_reg() & _BV(pin);
    }

    reg8_t port_reg() const {
        return port;
    }

    reg8_t ddr_reg() const {
        return *(&port + (&DDRB - &PORTB));
    }

    reg8_t pin_reg() const {
        return *(&port + (&PINB - &PORTB));
    }

    Pin const & operator=(bool const value) {
        set(value);
        return *this;
    }

    operator bool() const {
        return get();
    }


};

#endif
