#ifndef POS_SENSOR_H
#define POS_SENSOR_H

#include <stdint.h>

#include <avr/io.h>
#include <util/delay.h>

void adc_init() {
    ADCSRA = _BV(ADEN);
    DIDR0 = 0xFF;
    DIDR2 = 0xFF;
}

enum SensorError {
    NONE = 0,
    OVERRANGE = 1,
    UNDERRANGE = 2
};

struct Sensor {
    uint8_t admux;
    int16_t
        min_adc,
        max_adc,
        min_deg,
        max_deg;

    int16_t read_adc() {
        ADCSRA = 0;
        ADMUX = (0x1F & admux) | _BV(REFS0);
        ADCSRB = (admux & 0x20) ? 0x08 : 0;
        _delay_us(2);
        ADCSRA = _BV(ADEN);
        _delay_us(2);
        ADCSRA |= _BV(ADSC);
        while (ADCSRA & _BV(ADSC));
        return ADC;
    }

    SensorError check(int16_t const adc) const {
        if (adc < min_adc)
            return UNDERRANGE;

        if (adc > max_adc)
            return OVERRANGE;

        return NONE;
    }

    int16_t translate(int16_t const adc) {
        return 0; // map(adc, min_adc, max_adc, min_deg, max_deg);
        // TODO
    }
};

#endif

