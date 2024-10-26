#include <iostream>
#include <memory>
#include "TGA.hpp"
#include <SDL.h>
#include "SDLProgram.hpp"

#if _WIN32
#if 1
constexpr auto s_backgroundFilepath = "../resources/overpass-graffiti.tga";
constexpr auto s_particleFilepath = "../resources/random.tga";
#else
constexpr auto s_backgroundFilepath = "../../resources/small-red-box.tga";
constexpr auto s_particleFilepath = "../../resources/small-blue-area.tga";
#endif
#else
#if 1
constexpr auto s_backgroundFilepath = "../../resources/overpass-graffiti.tga";
constexpr auto s_particleFilepath = "../../resources/random.tga";
#else
constexpr auto s_backgroundFilepath = "../../resources/small-red-box.tga";
constexpr auto s_particleFilepath = "../../resources/small-blue-area.tga";
#endif
#endif

int main(int argc, char** argv) {
    std::unique_ptr<SDLProgram> program = std::make_unique<SDLProgram>();

    if(program->init(640, 480) == false) {
        return 1;
    }

    // 리소스 로드
    std::unique_ptr<TGA> bg = std::make_unique<TGA>();
    if(bg->readFromFile(s_backgroundFilepath) == false) {
        std::cout << "File read failed " << s_backgroundFilepath << std::endl;
        return 1;
    }
    if(bg->createTexture(program->nativeRenderer()) == false) {
        std::cout << "Create texture failed " << s_backgroundFilepath << std::endl;
        return 1;
    }
    std::unique_ptr<TGA> particle = std::make_unique<TGA>();
    if(particle->readFromFile(s_particleFilepath) == false) {
        std::cout << "File read failed " << s_particleFilepath << std::endl;
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
        // TODO: Draw with alpha blending
//        program->renderer()->enableBlending(SDL_BLENDMODE_BLEND);
//        program->renderer()->drawTGA(particle, 100, 100);
//        program->renderer()->flush();
        
        // TODO: Draw with additive blending
        program->renderer()->enableBlending(SDL_BLENDMODE_ADD);
        program->renderer()->drawTGA(particle, 200, 100);
        program->renderer()->flush();
        
        // TODO: Draw with multiply blending
//        program->renderer()->enableBlending(SDL_BLENDMODE_M UL);
//        program->renderer()->drawTGA(particle, 300, 100);
//        program->renderer()->flush();

        // Render
        program->renderer()->present();

        // TODO: Add sleep
    }

    return 0;
}
