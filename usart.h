#ifndef USART_H
#define USART_H

typedef volatile uint8_t& reg_t;

struct USART_t {
    reg_t UDR,
          UCSRA,
          UCSRB,
          UCSRC;
    volatile uint16_t& UBRR;
};



template <int N> 
USART_t get_USART() {
    switch(N) {
        default:
        case 0: return USART_t { UDR0, UCSR0A, UCSR0B, UCSR0C, UBRR0 };
        case 1: return USART_t { UDR1, UCSR1A, UCSR1B, UCSR1C, UBRR1 };
        case 2: return USART_t { UDR2, UCSR2A, UCSR2B, UCSR2C, UBRR2 };
        case 3: return USART_t { UDR3, UCSR3A, UCSR3B, UCSR3C, UBRR3 };
    }
}

template <typename T, int SIZE>
class CircularBuffer {
    private:
        T buffer[SIZE];
        int head;
        int tail;
        void advance(int& number) {
            number = (number + 1) % SIZE;
        }
    public:
        CircularBuffer():
            head(0), tail(0) {}
        int length() const {
            return (head + SIZE - tail) % SIZE;
        }
        void push(const T& value) {
            if(length() < SIZE ) {
                buffer[head] = value;
                advance(head);
            }
            else {
                // discard overflow text
            }
        }
        T pop() {
            if(length() > 0)
            {
                T& result = buffer[tail];
                advance(tail);
                return result;
            }
            else
            {
                return buffer[0];
            }
        }

};

template <int N, int BUFSIZE=16>
class USART {
    private:
        USART_t usart;
        //CircularBuffer<uint8_t, BUFSIZE> tx_buffer;
        //CircularBuffer<uint8_t, BUFSIZE> rx_buffer;

    public:
        USART(long baud = 9600):
            usart(get_USART<N>())
        {
            select_baudrate(baud);
            usart.UCSRB = _BV(RXEN0) | _BV(TXEN0);
            usart.UCSRC = _BV(UCSZ00) | _BV(UCSZ01);
        }
        void select_baudrate(long baud) {
            // calculate baud
            usart.UBRR = F_CPU / 16 / baud - 1;
        }
        void send(uint8_t value) {
            while(!(usart.UCSRA & _BV(UDRE0)));
            usart.UDR = value;

        }
        void print(const char* value) {
            for(int i=0; value[i]; i++)
                send(value[i]);
        }
        void print(int number) {
            int size = number ? 0 : 1;
            int ncopy = number;
            char output[10];
            for(;ncopy;) {
                ncopy /= 10;
                size++;
            }
            ncopy = number;
            for(int i=0;ncopy;i++) {
                output[size - 1 - i] = ncopy % 10 + '0';
                ncopy /= 10;
            }
            for(int i=0; i<size; i++)
                send(output[i]);
        }

};

#endif
