//
// Created by feder34 on 02/09/24.
//
#include "../include/Renderer.h"

bool CHIP8_RENDERER::init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }

        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }

        }
    }

    return success;
}


void CHIP8_RENDERER::close()
{
    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

CHIP8_RENDERER::CHIP8_RENDERER()
{
    init();
}

CHIP8_RENDERER::~CHIP8_RENDERER()
{
    close();
}

void CHIP8_RENDERER::draw()
{
    //Sets the color to clear the screen
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
    //Clears the Screen
    SDL_RenderClear(gRenderer);
    int rectBaseX = 0;
    int rectWidth = SCREEN_WIDTH/CHIP8X;
    int rectBaseY = 0;
    int rectHeight = SCREEN_HEIGHT/CHIP8Y;

    for(int i = 0; i < CHIP8Y; i++)
    {
        for(int j = 0; j < CHIP8X; j++)
        {
            SDL_Rect fillRect = {rectBaseX + rectWidth * j, rectBaseY + rectHeight * i, rectWidth, rectHeight};

            if(display[i * CHIP8X + j])
            {
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            }
            else
            {
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
            }
            SDL_RenderFillRect( gRenderer, &fillRect );
        }
    }

    //Update screen
    SDL_RenderPresent( gRenderer );


}

void CHIP8_RENDERER::displayFlip(uint32_t pos) {
    this->display[pos] = !this->display[pos];
}

void CHIP8_RENDERER::resetDisplay() {

    memset(this->display, 0, CHIP8X*CHIP8Y);

}

bool CHIP8_RENDERER::getDisplayStatus(uint32_t pos) {
    return this->display[pos];
}
