//
// Created by feder34 on 02/09/24.
//

#ifndef CHIP8_EMU_STACK_H
#define CHIP8_EMU_STACK_H
#include <cstdint>


struct stack
{
    int16_t mem[4096];

    int16_t stackPointer = 0;

    int16_t pop()
    {
        stackPointer--;
        return mem[stackPointer];
    }

    void push(uint16_t el)
    {
        mem[stackPointer] = el;
        stackPointer++;
    }

};


#endif //CHIP8_EMU_STACK_H
