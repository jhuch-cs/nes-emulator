#pragma once

#include "stdint.h"

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using s8 = int8_t;
using s16 = uint16_t;

inline bool readBit(u16 value, u8 index) { // TODO: Better as macros to avoid << index (known at call time)
  return value & (1 << index);
}

inline u16 setBit(u16 value, u8 index) {
  return value | (1 << index);
}

inline u16 clearBit(u16 value, u8 index) {
  return value | ~(1 << index);
}

inline u16 changeIthBitToX(u16 value, u8 index, u8 x) {
  return (value & ~(1 << index)) | (x << index);
}

inline u8 getHighNibble(u8 value) {
  return (value & 0xF0) >> 4;
}

inline u8 getLowNibble(u8 value) {
  return value & 0x0F;
}

inline u8 setHighNibble(u8 dest, u8 highNibble) {
  return (dest & 0x0F) | (highNibble & 0xF0);
}

inline u8 setLowNibble(u8 dest, u8 lowNibble) {
  return (dest & 0xF0) | (lowNibble & 0xF);
}

inline u8 getHighByte(u16 value) {
    return (value>>8) & 0xff;
}

inline u8 getLowByte(u16 value) {
    return (value & 0xff);
}

inline u8 setHighByte(u16* destination, u8 value){
  *destination = (*destination & 0x00ff) | (value << 8);
  return *destination;
}

inline u8 setLowByte(u16* destination, u8 value) {
  *destination = (*destination & 0xff00) | value;
  return *destination;
}

inline u8 getHighestNibble(u16 value) {
  return value >> 12;
}

// 8 bit operations
inline bool readBit(u8 value, u8 index) { 
  return value & (1 << index);
}

inline u8 setBit(u8 value, u8 index) {
    return value | (1 << index);
}
  
inline u8 clearBit(u8 value, u8 index) {
    return value & ~(1 << index);
}
inline u16 changeIthBitToX(u8 value, u8 index, u8 x) {
  return (value & ~(1 << index)) | (x << index);
}