#ifndef REGISTER_H
#define REGISTER_H

#include <stdint.h>

template <typename T>
using reg_t = volatile T &;

using reg8_t  = reg_t<uint8_t>;
using reg16_t = reg_t<uint16_t>;

#endif