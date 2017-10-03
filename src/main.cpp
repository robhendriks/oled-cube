#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <json.hpp>

#include "math.h"
#include "world.h"
#include "graphics.h"

SDL_Window *gWindow = nullptr;
SDL_Renderer *gRenderer = nullptr;

#define WIDTH 128
#define HEIGHT 48

int main(int argc, const char * argv[]) {
    //
    // MATH SHIT
    
    Graphics::BackBuffer backBuffer{WIDTH, HEIGHT};
    
    World::Camera camera;
    camera.setPosition({0, 10, 10});
    camera.setTarget({0, 0, 0});
    
    // TODO: Clear this utter mess up pls
    std::ifstream is("/Users/rob/Desktop/Torus.babylon");
    nlohmann::json j = nlohmann::json::parse(is);
    
    std::vector<Math::Vector> vertices;
    std::vector<World::Face> faces;
    
    nlohmann::json jMesh = j["meshes"][0];
    nlohmann::json jVertices = jMesh["vertices"];
    nlohmann::json jIndices = jMesh["indices"];
    
    int vertexStep = 1;
    int uvCount = jMesh["uvCount"].get<int>();
    
    switch (uvCount) {
        case 0:
            vertexStep = 6; break;
        case 1:
            vertexStep = 8; break;
        case 2:
            vertexStep = 10; break;
    }
    
    std::size_t vertexCount = jVertices.size() / vertexStep;
    std::size_t indiceCount = jIndices.size() / 3;
    
    vertices.reserve(vertexCount);
    faces.reserve(indiceCount);
    
    for (std::size_t i = 0; i < vertexCount; ++i) {
        float x = jVertices[i * vertexStep].get<float>();
        float y = jVertices[i * vertexStep + 1].get<float>();
        float z = jVertices[i * vertexStep + 2].get<float>();
        vertices.push_back(Math::Vector(x, y, z));
    }
    
    for (std::size_t i = 0; i < indiceCount; ++i) {
        int a = jIndices[i * 3].get<int>();
        int b = jIndices[i * 3 + 1].get<int>();
        int c = jIndices[i * 3 + 2].get<int>();
        faces.push_back(World::Face{a, b, c});
    }
    
    // TODO: Write importer class
    // TODO: Improve mesh class
    
    World::Mesh cubeMesh{"Cube", vertices, faces};
    cubeMesh.setPosition({0, 0, 0});
    cubeMesh.setRotation({0, 0, 0});
    
    //
    // SDL SHIT
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Error: #001\n";
        return 1;
    }
    
    gWindow = SDL_CreateWindow("Cube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
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
        
        cubeMesh.rotation().x += 0.01; // pitch
        cubeMesh.rotation().y += 0.02; // yaw
        cubeMesh.rotation().z += 0.01; // roll
        
        backBuffer.clear({0, 0, 0, 1});
        camera.render(backBuffer, cubeMesh, {1, 1, 1, 1});
        
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
