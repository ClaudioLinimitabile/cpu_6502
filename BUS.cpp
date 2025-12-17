#include "BUS.h"
#include <fstream>
#include <iostream>

uint8_t BUS::read(uint16_t addr){
    return RAM[addr];
}

void BUS::write(uint16_t addr, uint8_t value){
    RAM[addr] = value;
}

void BUS::LoadROM(const char* filename, uint16_t loadAddress){
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if(!file){
        std::cerr << "ROM NOT FOUND\n";
        return;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if(loadAddress + size > 65536){
        std::cerr << "ROM TOO BIG\n";
        return;
    }

    file.read((char*)&RAM[loadAddress], size);
    std::cout << "ROM LOADED: " << size << " bytes\n";
}