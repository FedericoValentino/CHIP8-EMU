//
// Created by feder34 on 02/09/24.
//
#ifndef CHIP8_EMU_PROCESSOR_H
#define CHIP8_EMU_PROCESSOR_H

#include <chrono>
#include <cstdint>
#include "stack.h"
#include "Renderer.h"


enum STAGE{
    FETCH,
    DECODE,
    EXECUTE
};

class Processor {
private:

    //architecture stuff
    uint8_t memory[4096] = {0xF0, 0x90, 0x90, 0x90, 0xF0,       // 0
                            0x20, 0x60, 0x20, 0x20, 0x70,       // 1
                            0xF0, 0x10, 0xF0, 0x80, 0xF0,       // 2
                            0xF0, 0x10, 0xF0, 0x10, 0xF0,       // 3
                            0x90, 0x90, 0xF0, 0x10, 0x10,   // 4
                            0xF0, 0x80, 0xF0, 0x10, 0xF0,   // 5
                            0xF0, 0x80, 0xF0, 0x90, 0xF0,   // 6
                            0xF0, 0x10, 0x20, 0x40, 0x40,   // 7
                            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
                            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
                            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
                            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
                            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
                            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
                            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
                            0xF0, 0x80, 0xF0, 0x80, 0x80 }; // F
    uint8_t registers[16];
    bool keys[16];
    uint16_t ProgramCounter;
    uint16_t I;
    stack mStack;
    int8_t delayTimer;
    int8_t soundTimer;

    uint16_t instruction;

    uint8_t OPCODE;
    uint16_t NNN;
    uint8_t X;
    uint8_t NN;
    uint8_t Y;
    uint8_t N;


    //software stuff
    CHIP8_RENDERER* CHIP8_Screen;

    int stage;

    void fetch();

    void decode();

    void execute();

    void drawProcedure();

public:

    Processor();

    void run();

    void loadRom(FILE* file);

};


#endif //CHIP8_EMU_PROCESSOR_H
