//
// Created by feder34 on 02/09/24.
//
#ifndef CHIP8_EMU_RENDERER_H
#define CHIP8_EMU_RENDERER_H

#include <SDL.h>

constexpr uint32_t SCREEN_WIDTH = 1280;
constexpr uint32_t SCREEN_HEIGHT = 640;
constexpr uint32_t CHIP8Y = 32;
constexpr uint32_t CHIP8X = 64;

class CHIP8_RENDERER
{
private:
    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;

    bool display[CHIP8Y*CHIP8X] = {0};

private:
    bool init();
    void close();

public:
    CHIP8_RENDERER();

    void draw();

    void displayFlip(uint32_t pos);

    void resetDisplay();

    bool getDisplayStatus(uint32_t pos);

    ~CHIP8_RENDERER();
};


#endif //CHIP8_EMU_RENDERER_H
