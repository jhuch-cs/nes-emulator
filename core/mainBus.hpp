#pragma once

#include "./util.hpp"

class MainBus {
public:
    MainBus();

    u8 read(u16 addr);
    void write(u16 addr, u8 val);
    u8* getPointerTo(u16 addr);
private:
    u8 ram[0x800]; // 0x0000 - 0x07FF, mirrored in 0x800-0x1FFF
    u8* bogusPointer = new u8[1];
};