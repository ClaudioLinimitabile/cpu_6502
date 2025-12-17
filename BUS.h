#pragma once
#include <cstdint>

class BUS{
    public:
    uint8_t RAM[64*1024];

    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t value);

    void LoadROM(const char* filename, uint16_t loadAddress);
};