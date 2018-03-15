#ifndef POS_SENSOR_H
#define POS_SENSOR_H
#include <avr/io.h>
#include <util/delay.h>

void adc_init() {
  ADCSRA = _BV(ADEN);
  DIDR0 = 0xFF;
  DIDR2 = 0xFF;
}
enum SensorError : int {
  NONE = 0,
  OVERRANGE = 1,
  UNDERRANGE = 2
};

struct Sensor {
  uint8_t admux;
  int min_adc,
      max_adc,
      min_deg,
      max_deg;

  int read_adc() {
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

  SensorError check(int adc) {
    if (adc < min_adc) {
      return UNDERRANGE;
    }
    else if (adc > max_adc) {
      return OVERRANGE;
    }
    return NONE;
  }

  int translate(int adc) {
    return 0; // map(adc, min_adc, max_adc, min_deg, max_deg);
    // TODO
  }
};



#endif

