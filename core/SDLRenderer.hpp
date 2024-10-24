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
    
    void clear();

    void drawTGA(std::unique_ptr<TGA>& tga, int x, int y);
    
    void enableBlending(SDL_BlendMode blendMode);
    
    void disableBlending();

    void present();
    
    void flush();

private:
    SDL_Renderer* m_renderer = nullptr;
    
    SDL_BlendMode m_currentBlendMode = SDL_BLENDMODE_NONE;
};
