//------------------------------------------------------------------------------
// File: SDLRenderer.cpp
// Author: Chris Redwood
// Created: 2024-10-21
// License: MIT License
//------------------------------------------------------------------------------

#include "SDLRenderer.hpp"

SDLRenderer::SDLRenderer(SDL_Window* window) {
    // TODO: 과연 생성자에 결과를 확인해야 할수도 있는 인스턴스를 초기화하는 것이 좋을까?
    m_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(m_renderer == nullptr) {
        SDL_assert(false);
    }
}

SDLRenderer::~SDLRenderer() {
    SDL_DestroyRenderer(m_renderer);
}

SDL_Renderer* SDLRenderer::native() {
    return m_renderer;
}

// TODO: Alpah Blend

// TODO: Multiply Blend

// TODO: Subtract Blend

void SDLRenderer::drawTGA(std::unique_ptr<TGA>& tga, int x, int y) {
    // Render TGA

    // Blend
}

void SDLRenderer::present() {
    SDL_RenderPresent(m_renderer);
}