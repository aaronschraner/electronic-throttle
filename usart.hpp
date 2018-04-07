#ifndef USART_H
#define USART_H

#include <stdint.h>
#include <stddef.h>

#include "register.hpp"

struct USART_t {
    reg8_t UDR,
          UCSRA,
          UCSRB,
          UCSRC;
    reg16_t UBRR;
};

template <size_t INDEX> 
USART_t get_USART() {
    switch(INDEX) {
        default:
        case 0: return USART_t { UDR0, UCSR0A, UCSR0B, UCSR0C, UBRR0 };
        case 1: return USART_t { UDR1, UCSR1A, UCSR1B, UCSR1C, UBRR1 };
        case 2: return USART_t { UDR2, UCSR2A, UCSR2B, UCSR2C, UBRR2 };
        case 3: return USART_t { UDR3, UCSR3A, UCSR3B, UCSR3C, UBRR3 };
    }
}

template <typename T, size_t SIZE>
class CircularBuffer {
private:
    T buffer[SIZE];
    size_t head, tail;
    void advance(int& number) {
        number = (number + 1) % SIZE;
    }
public:
    CircularBuffer() : head{0}, tail{0} {}
    
    size_t length() const {
        return (head + SIZE - tail) % SIZE;
    }

    void push(const T& value) {
        if (length() >= SIZE)
            return; // Discard overflow text

        buffer[head] = value;
        advance(head);
    }

    T pop() {
        if(length() == 0)
            return buffer[0];
        
        T& result = buffer[tail];
        advance(tail);
        return result;
    }
};

template <size_t N, size_t BUFSIZE=16>
class USART {
private:
    USART_t usart;
    //CircularBuffer<uint8_t, BUFSIZE> tx_buffer;
    //CircularBuffer<uint8_t, BUFSIZE> rx_buffer;

public:
    USART(uint32_t const baud = 9600) :
        usart(get_USART<N>())
    {
        select_baudrate(baud);
        usart.UCSRB = _BV(RXEN0) | _BV(TXEN0);
        usart.UCSRC = _BV(UCSZ00) | _BV(UCSZ01);
    }

    void select_baudrate(uint32_t const baud) {
        // calculate baud
        usart.UBRR = F_CPU / 16 / baud - 1;
    }

    void send(uint8_t const value) {
        while(!(usart.UCSRA & _BV(UDRE0)));
        usart.UDR = value;
    }

    void print(char const * value) {
        for(; *value; value++)
            send(*value);
    }

    void print(int16_t const number) {
        uint8_t num_digits = number ? 0 : 1;
        char output[10];

        for(int16_t dec_shifted{number}; dec_shifted; dec_shifted /= 10)
            num_digits++;
        
        for(int16_t dec_shifted{number}, i{0}; dec_shifted; i++, dec_shifted /= 10)
            output[num_digits - 1 - i] = dec_shifted % 10 + '0';
        
        for(int16_t i{0}; i < num_digits; i++)
            send(output[i]);
    }
};

#endif
