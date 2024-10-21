//------------------------------------------------------------------------------
// File: SDLRenderer.hpp
// Author: Chris Redwood
// Created: 2024-10-21
// License: MIT License
//------------------------------------------------------------------------------

#pragma once
#include <memory>
#include <SDL.h>
#include "TGA.hpp"

class SDLRenderer {
public:
    SDLRenderer() = delete;

    SDLRenderer(SDL_Window* window);

    ~SDLRenderer();

    SDL_Renderer* native();

    void drawTGA(std::unique_ptr<TGA>& tga, int x, int y);

    void present();

private:
    SDL_Renderer* m_renderer = {};
};