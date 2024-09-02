//
// Created by feder34 on 02/09/24.
//
#include <stdio.h>
#include "include/Renderer.h"
#include "include/Processor.h"



int main(int argc, char** argv)
{

    FILE* rom = fopen(argv[1], "rb");
    auto* CHIP8 = new Processor();
    CHIP8->loadRom(rom);

    CHIP8->run();

}