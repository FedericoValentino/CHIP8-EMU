//
// Created by feder34 on 02/09/24.
//

#include "../include/Processor.h"

void Processor::fetch() {
    uint16_t instr1 = memory[ProgramCounter];
    instr1 = instr1 << 8;
    uint16_t instr2 = memory[ProgramCounter+1];
    instruction = instr1 | instr2;
    ProgramCounter += 2;
    this->stage = DECODE;
}

void Processor::decode() {
    OPCODE = (instruction>>12) & 0xF;
    NNN = (instruction)& 0x0FFF;
    X = (instruction >> 8) & 0x0F;
    NN = (instruction) & 0xFF;
    Y = (instruction>>4) & 0xF;
    N = instruction & 0xF;
    this->stage = EXECUTE;
}

void Processor::execute() {

    bool pressed = false;
    switch(OPCODE)
    {
        case 0x0:
            if(Y == 0xE)
            {
                if(N == 0xE)
                {
                    ProgramCounter = mStack.pop();
                }
                else
                {
                    CHIP8_Screen->resetDisplay();
                    CHIP8_Screen->draw();
                }
            }
            break;
        case 0x1:
            ProgramCounter = NNN;
            break;
        case 0x2:
            mStack.push(ProgramCounter);
            ProgramCounter = NNN;
            break;
        case 0x3:
            if(registers[X] == NN)
            {
                ProgramCounter += 2;
            }
            break;
        case 0x4:
            if(registers[X] != NN)
            {
                ProgramCounter += 2;
            }
            break;
        case 0x5:
            if(registers[X] == registers[Y])
            {
                ProgramCounter += 2;
            }
            break;
        case 0x6:
            registers[X] = NN;
            break;
        case 0x7:
            registers[X] += NN;
            break;
        case 0x8:
            switch(N)
            {
                case 0x0:
                    registers[X] = registers[Y];
                    break;
                case 0x1:
                    registers[X] |= registers[Y];
                    break;
                case 0x2:
                    registers[X] &= registers[Y];
                    break;
                case 0x3:
                    registers[X] ^= registers[Y];
                    break;
                case 0x4:
                    registers[X] += registers[Y];
                    break;
                case 0x5:
                    registers[X] -= registers[Y];
                    break;
                case 0x6:
                    registers[X] = registers[Y];
                    registers[0xF] = registers[X] & 0x1;
                    registers[X] = registers[X] >> 1;
                    break;
                case 0x7:
                    registers[X] = registers[Y] - registers[X];
                    break;
                case 0xE:
                    registers[X] = registers[Y];
                    registers[0xF] = (registers[X] >> 15) & 0x1;
                    registers[X] = registers[X] << 1;
                    break;
            }
            break;
        case 0x9:
            if(registers[X] != registers[Y])
            {
                ProgramCounter += 2;
            }
            break;
        case 0xA:
            I = NNN;
            break;
        case 0xB:
            //make this configurable
            ProgramCounter = NNN + registers[0];
            break;
        case 0xC:
            registers[X] = rand() & NN;
            break;
        case 0xD:
            drawProcedure();
            break;
        case 0xE:
            if(Y == 0x9)
            {
                if(keys[registers[X]])
                {
                    ProgramCounter+=2;
                }
            }
            else if(Y == 0xA)
            {
                if(!keys[registers[X]])
                {
                    ProgramCounter+=2;
                }
            }
            break;
        case 0xF:
            switch(NN)
            {
                case 0x07:
                    registers[X] = delayTimer;
                    break;
                case 0x15:
                    delayTimer = registers[X];
                    break;
                case 0x18:
                    soundTimer = registers[X];
                    break;
                case 0x1E:
                    I += registers[X];
                    break;
                case 0x0A:
                    pressed = false;
                    for(int i = 0; i < 16; i++)
                    {
                        if(keys[i])
                            pressed = true;
                    }
                    if(!pressed)
                        ProgramCounter--;
                    break;
                case 0x29:
                    I = 5u * registers[X];
                    break;
                case 0x33:
                    memory[I] = (unsigned char)(registers[X]) / 100;
                    memory[I+1] = ((unsigned char)(registers[X]) / 10) % 10;
                    memory[I+2] = (unsigned char)(registers[X]) % 10;
                    break;
                case 0x55:
                    for(int i = 0; i <= X; i++)
                    {
                        memory[I+i] = registers[i];
                    }
                    break;
                case 0x65:
                    for(int i = 0; i <= X; i++)
                    {
                        registers[i] = memory[I+i];
                    }
                    break;

            }
            break;
    }
    this->stage = FETCH;
}

void Processor::run() {
    static auto startTime = std::chrono::high_resolution_clock::now();
    float lastTime = 0;
    float accumulator = 0;




    CHIP8_Screen = new CHIP8_RENDERER();
    bool quit = false;
    SDL_Event e;
    while( !quit )
    {
        //Handle events on queue
        while( SDL_PollEvent(&e) != 0 )
        {
            //User requests quit
            if(e.type == SDL_QUIT )
            {
                quit = true;
            }
        }
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
        auto deltaT = time - lastTime;
        lastTime = time;
        accumulator += deltaT;

        if(accumulator > 0.01f)
        {
            accumulator = 0.0f;
            switch (this->stage)
            {
                case FETCH:
                    fetch();
                    break;
                case DECODE:
                    decode();
                    break;
                case EXECUTE:
                    execute();
                    break;
            }
        }

    }

}

void Processor::drawProcedure()
{
    int realX = registers[X] % 64;
    int realY = registers[Y] % 32;
    registers[0xF] = 0;
    for(int i = 0; i < N; i++)
    {
        uint8_t spriteData = memory[I + i];
        for(int j = 0; j < 8; j++)
        {
            if(realX+j < 64)
            {
                bool mostSignificant = (spriteData >> (7 - j)) & 0x1;
                if (mostSignificant) {
                    registers[0xF] = CHIP8_Screen->getDisplayStatus(realY * 64 + realX);
                    CHIP8_Screen->displayFlip(realY * 64 + realX + j);
                }
            }
        }
        realY++;
    }
    CHIP8_Screen->draw();
}

Processor::Processor() {
    ProgramCounter = 0x200;
}

void Processor::loadRom(FILE* file) {

    int count = 0;
    while(!feof(file))
    {
        uint8_t instruction;
        fread(&instruction, sizeof(unsigned char), 1, file);
        memory[0x200+count] = instruction;
        //printf("%x\n", instruction);
        count++;
    }


}

