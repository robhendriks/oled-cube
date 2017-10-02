#include <iostream>
#include <SDL2/SDL.h>

#include "math.h"
#include "world.h"

#include "graphics.h"

SDL_Window *gWindow = nullptr;
SDL_Renderer *gRenderer = nullptr;

int main(int argc, const char * argv[]) {
    
    //
    // MATH SHIT
    
    Graphics::BackBuffer backBuffer{640, 480};
    
    World::Camera camera;
    camera.setPosition({0, 0, 10});
    camera.setTarget({0, 0, 0});
    
    std::vector<Math::Vector> cubeVertices = {
        {-1, 1, 1},
        {1, 1, 1},
        {-1, -1, 1},
        {1, -1, 1},
        {-1, 1, -1},
        {1, 1, -1},
        {1, -1, -1},
        {-1, -1, -1},
    };
    
    std::vector<World::Face> cubeFaces = {
        {0, 1, 2},
        {1, 2, 3},
        {1, 3, 6},
        {1, 5, 6},
        {0, 1, 4},
        {1, 4, 5},
        {2, 3, 7},
        {3, 6, 7},
        {0, 2, 7},
        {0, 4, 7},
        {4, 5, 6},
        {4, 6, 7},
    };
    
    World::Mesh cubeMesh{"Cube", cubeVertices, cubeFaces};
    
    backBuffer.clear({0, 0, 0, 1});
    
    camera.render(backBuffer, cubeMesh, {1, 0, 1, 1});
    
    //
    // SDL SHIT
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Error: #001\n";
        return 1;
    }
    
    gWindow = SDL_CreateWindow("Cube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    if (!gWindow) {
        std::cerr << "Error: #002\n";
        return 1;
    }
    
    gRenderer = SDL_CreateRenderer(gWindow, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!gRenderer) {
        std::cerr << "Error: #003\n";
        return 1;
    }
    
    bool quit = false;
    SDL_Event e;
    
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(gRenderer);
        
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
        SDL_Rect rect{0, 0, 128, 64};
        SDL_RenderFillRect(gRenderer, &rect);
        
        const std::vector<uint8_t>& buffer = backBuffer.buffer();
        uint32_t j = 0;
        for (uint32_t i = 0; i < backBuffer.size(); i += 4) {
            SDL_SetRenderDrawColor(gRenderer, buffer[i], buffer[i + 1], buffer[i + 2], buffer[i + 3]);
            
            uint16_t x, y;
            x = j % backBuffer.width();
            y = j / backBuffer.width();
            
            SDL_RenderDrawPoint(gRenderer, x, y);
            ++j;
        }
        
        SDL_RenderPresent(gRenderer);
    }
    
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
    
    return 0;
}
