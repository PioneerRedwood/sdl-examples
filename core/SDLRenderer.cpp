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

void SDLRenderer::clear() {
    SDL_RenderClear(m_renderer);
}

void SDLRenderer::enableBlending(SDL_BlendMode blendMode) {
    m_currentBlendMode = blendMode;
}

void SDLRenderer::disableBlending() {
    m_currentBlendMode = SDL_BLENDMODE_NONE;
}

// TODO: Alpah Blend

// TODO: Multiply Blend

// TODO: Subtract Blend

void SDLRenderer::drawTGA(std::unique_ptr<TGA>& tga, int x, int y) {
    SDL_Rect rect = {0};
    rect.x = x, rect.y = y, rect.w = tga->header()->width, rect.h = tga->header()->height;
    
    switch(m_currentBlendMode) {
        case SDL_BLENDMODE_ADD: {
            break;
        }
        case SDL_BLENDMODE_MUL: {
            break;
        }
        case SDL_BLENDMODE_: {
            break;
        }
        default: {
            SDL_RenderCopy(m_renderer, const_cast<SDL_Texture*>(tga->sdlTexture()), nullptr, &rect);
            break;
        }
    }
}

void SDLRenderer::present() {
    SDL_RenderPresent(m_renderer);
}
