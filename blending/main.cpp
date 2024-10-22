#include <iostream>
#include <memory>
#include "TGA.hpp"
#include <SDL.h>
#include "SDLProgram.hpp"

constexpr auto s_backgroundFilepath = "../../resources/overpass-graffiti.tga";
constexpr auto s_particleFilepath = "../../resources/particle.tga";

int main(int argc, char** argv) {
    std::unique_ptr<SDLProgram> program = std::make_unique<SDLProgram>();

    if(program->init(640, 480) == false) {
        return 1;
    }

    // 리소스 로드
    std::unique_ptr<TGA> bg = std::make_unique<TGA>();
    if(bg->readFromFile(s_backgroundFilepath) == false) {
        return 1;
    }
    if(bg->createTexture(program->nativeRenderer()) == false) {
        return 1;
    }
    std::unique_ptr<TGA> particle = std::make_unique<TGA>();
    if(particle->readFromFile(s_particleFilepath) == false) {
        return 1;
    }
    if(particle->createTexture(program->nativeRenderer()) == false) {
        return 1;
    }

    // Main loop
    while(program->neededQuit() == false) {
        // Update delta

        SDL_Event event;
        while(SDL_PollEvent(&event) != 0) {
            program->handleEvent(&event);
        }

        // Update
        
        // Draw background first
        program->renderer()->drawTGA(bg, 0, 0);
        
        // Draw the instance next
        program->renderer()->drawTGA(particle, 100, 100);

        // Render
        program->renderer()->present();
    }

    return 0;
}
