//------------------------------------------------------------------------------
// File: SDLProgram.hpp
// Author: Chris Redwood
// Created: 2024-10-21
// License: MIT License
//------------------------------------------------------------------------------

#pragma once

#include <memory>
#include <SDL.h>
#include "SDLRenderer.hpp"

class SDLProgram {
public:
    SDLProgram() = default;

    ~SDLProgram();

    bool init(int width, int height);

    void quit();

    SDL_Window* window();

    SDL_Renderer* nativeRenderer();

    std::unique_ptr<SDLRenderer>& renderer();

    bool neededQuit() const;

    void handleEvent(SDL_Event* event);

private:
    SDL_Window* m_window = nullptr;

    std::unique_ptr<SDLRenderer> m_renderer;

    bool m_neededQuit = false;
};