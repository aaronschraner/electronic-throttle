#ifndef CAN_H
#define CAN_H
#include "can_ref.hpp"

#include <pin.hpp>

struct PinHolder {
    Pin cs;
    bool startvalue;
    PinHolder(const Pin& cs, bool value = 0): 
        cs(cs), startvalue(cs) { 
        cs = value; 
    }
    ~PinHolder() { cs = startvalue; }
};

class CAN {
    private:
        Pin cs_pin;

    public:
        CAN(Pin const & cs_pin):
            cs_pin(cs_pin) {
            cs_pin.mode(OUTPUT);
        }
        void init() {
        }

        void write_reg(uint8_t address, uint8_t mask, uint8_t value) {
            PinHolder ph(cs_pin);

            spi_send(INSTR_BIT_MODIFY);
            spi_send(address);
            spi_send(mask);
            spi_send(value);
        }

        uint8_t read_reg(uint8_t address) {
            // pull CS low
            PinHolder ph(cs_pin);

            spi_send(INSTR_READ);
            spi_send(address);

            return spi_send(0x00);
            // CS returned to high automatically
        }

        uint8_t get_status() {
            uint8_t status, status_copy;
            do {
                // pull CS low
                PinHolder ph(cs_pin);
                spi_send(INSTR_READ_STATUS);
                status = spi_send(0x00);
                status_copy = spi_send(0x00);
            } while(status ^ status_copy);
            return status;
        }

        void write_reg(uint8_t address, uint8_t value) {
            PinHolder ph(cs_pin);
            spi_send(INSTR_WRITE);
            spi_send(address);
            spi_send(value);
        }

        // user-facing stuff

        void begin() {
            write_reg(REG_CANCTRL, 0x80);
            
            // 1Mbps baud rate, 16MHz clock
            write_reg(REG_CONF1, 0x00);
            write_reg(REG_CONF2, 0xd0);
            write_reg(REG_CONF3, 0x82);
        }

        template <typename T>
        void send(const T& data, uint16_t id) {
            // TODO: retry, error checking, etc.
            
            write_reg(REG_TXB0SIDH, id >> 3);
            write_reg(REG_TXB0SIDL, id << 5);
            write_reg(REG_TXB0EID8, 0x00);
            write_reg(REG_TXB0EID0, 0x00);
            write_reg(REG_TXB0DLC, sizeof(T));
            for(uint8_t i=0; i < sizeof(T); i++) {
                write_reg(REG_TXB0D0 + i, *((uint8_t*)(&data) + i));
            }

            write_reg(REG_TXB0CTRL, 0x08);
        }






};

#endif
