#include <avr/io.h>
#include <util/delay.h>

#include "pin.h"
#include "usart.h"
#include "pos_sensor.h"
#include "servo.h"



const uint16_t ADMUX_APPS1 = _BV(REFS0) | 0x00; // ADC0, single ended, 5V ref
const uint16_t ADMUX_APPS2 = _BV(REFS0) | 0x02; // ADC2, single ended, 5V ref
const uint16_t ADMUX_TPS1 = (_BV(MUX5) << 8) | _BV(REFS0) | 0x04; // ADC0, single ended, 5V ref
const uint16_t ADMUX_TPS2 = (_BV(MUX5) << 8) | _BV(REFS0) | 0x06; // ADC2, single ended, 5V ref

int read_channel(uint16_t admux) {
    ADCSRA = 0;
    ADMUX = admux & 0xFF;
    ADCSRB = admux >> 8;
    ADCSRA = _BV(ADEN);
    ADCSRA |= _BV(ADSC);
    while(ADCSRA & _BV(ADSC));
    return ADC;
}

void pullup_all_pins() {
    PORTA = 0xFF;
    PORTB = 0xFF;
    PORTC = 0xFF;
    PORTD = 0xFF;
    PORTE = 0xFF;
    PORTF = 0xFF;
    PORTG = 0xFF;
    PORTH = 0xFF;
    PORTK = 0xFF;
}
int main() {

    pullup_all_pins();
    
    USART<0> debug_usart(38400); //USART0, 38400 baud
    
    DDRJ = 0xFF;
    
    adc_init();
    
    servo_config();

    while(true)
    {

        {
            int ADC_APPS1 = read_channel(ADMUX_APPS1);
            int ADC_APPS2 = read_channel(ADMUX_APPS2);
            int ADC_TPS1  = read_channel(ADMUX_TPS1);
            int ADC_TPS2  = read_channel(ADMUX_TPS2);

            debug_usart.print("APPS1: ");
            debug_usart.print(ADC_APPS1);
            debug_usart.print(", APPS2: ");
            debug_usart.print(ADC_APPS2);
            debug_usart.print(", TPS1: ");
            debug_usart.print(ADC_TPS1);
            debug_usart.print(", TPS2: ");
            debug_usart.print(ADC_TPS2);
            debug_usart.print("\r\n");
        }
    }
}
