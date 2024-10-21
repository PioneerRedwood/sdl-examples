#include <iostream>
#include <memory>
#include "TGA.hpp"
#include <SDL.h>
#include "SDLProgram.hpp"

constexpr auto s_filepath = "../resources/particle.tga";

int main(int argc, char** argv) {
    std::unique_ptr<SDLProgram> program = std::make_unique<SDLProgram>();

    if(program->init(640, 480) == false) {
        return 1;
    }

    // 리소스 로드
    std::unique_ptr<TGA> tga = std::make_unique<TGA>();
    if(tga->readFromFile(s_filepath) == false) {
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
        program->renderer()->drawTGA(tga, 0, 0);

        // Render
        program->renderer()->present();
    }

    return 0;
}