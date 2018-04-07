#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>
#include <avr/io.h>

void servo_config() {
    static uint16_t const
        PRESCALE{8},
        FREQUENCY_HZ{50};

    // CTC mode, TOP=ICR1, 50Hz, adjustable duty cycle output on OC1B pin
    TCCR1A = _BV(COM1B1); // clear OC1B on c.m. when upcounting, set on c.m. when downcounting

    ICR1 = F_CPU / (FREQUENCY_HZ * 2 * PRESCALE); // set output frequency

    TCCR1B = _BV(WGM13); // phase and frequency correct PWM, clear at ICR1

    // set PRESCALE
    switch(PRESCALE) {
        case 1:    TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10); break;
        case 8:    TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10); break;
        case 64:   TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10); break;
        case 256:  TCCR1B |= (1 << CS12) | (0 << CS11) | (0 << CS10); break;
        case 1024: TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10); break;
        default:  break;
    }

    DDRB |= _BV(PB6); //enable OC1B pin as output
}

void servo_write(int16_t value) {
    // servo takes value between 0 and +/-1000
    // 5% duty cycle = -90deg, 10% duty cycle = +90deg
    uint16_t const
        BASE{250},
        DIVISOR{1},
        duty_cycle{BASE + value / DIVISOR}; // duty cycle out of 10000
    
    OCR1B = duty_cycle;
}

#endif
