#include <iostream>
#include <memory>
#include "TGA.hpp"
#include <SDL.h>
#include "SDLProgram.hpp"

#if 0
constexpr auto s_backgroundFilepath = "../../resources/overpass-graffiti.tga";
constexpr auto s_particleFilepath = "../../resources/random.tga";
#else
constexpr auto s_backgroundFilepath = "../../resources/small-red-box.tga";
constexpr auto s_particleFilepath = "../../resources/small-blue-area.tga";
#endif

int main(int argc, char** argv) {
    std::unique_ptr<SDLProgram> program = std::make_unique<SDLProgram>();

    if(program->init(640, 480) == false) {
        return 1;
    }

    // 리소스 로드
    std::unique_ptr<TGA> bg = std::make_unique<TGA>();
    // TODO: backgroud 리소스 잘못됐음 - 수정 바람
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
//    if(particle->createTexture(program->nativeRenderer()) == false) {
//        return 1;
//    }

    // Main loop
    while(program->neededQuit() == false) {
        // Update delta

        SDL_Event event;
        while(SDL_PollEvent(&event) != 0) {
            program->handleEvent(&event);
        }

        // Update
        SDL_SetRenderDrawColor(program->nativeRenderer(), 255, 255, 255, 255);
        program->renderer()->clear();
        
        // Draw background first
        program->renderer()->disableBlending();
        program->renderer()->drawTGA(bg, 0, 0);
        // Before read pixels from render target, render present
        program->renderer()->flush();
        
        // Draw the instance next
        program->renderer()->enableBlending(SDL_BLENDMODE_BLEND);
        program->renderer()->drawTGA(particle, 100, 100);
        
        // TODO: Draw with alpha blending

        // Render
        program->renderer()->present();

        // TODO: Add sleep
    }

    return 0;
}
