#ifndef SERVO_H
#define SERVO_H
void servo_config()
{
    const int prescale = 8;

    // CTC mode, TOP=ICR1, 50Hz, adjustable duty cycle output on OC1B pin
    TCCR1A = _BV(COM1B1); // clear OC1B on c.m. when upcounting, set on c.m. when downcounting

    const int frequency = 50; // Hz
    ICR1 = F_CPU / (frequency * 2 * prescale); // set output frequency


    TCCR1B = _BV(WGM13); // phase and frequency correct PWM, clear at ICR1

    // set prescale
    switch(prescale) {
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
    //servo takes value between 0 and +/-1000
    //5% duty cycle = -90deg, 10% duty cycle = +90deg
    const uint16_t duty_cycle = 250 + value / 1; // duty cycle out of 10000
    OCR1B = duty_cycle;
}

#endif
