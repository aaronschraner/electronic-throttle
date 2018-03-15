#include <avr/io.h>
#include <util/delay.h>

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
    DDRJ = 0xFF;
    while(true) {
        for(uint8_t x=1; x; x<<=1)
        {
            PORTJ = x;
            DDRJ = PORTJ;
            _delay_ms(100);
        }
    }
}
