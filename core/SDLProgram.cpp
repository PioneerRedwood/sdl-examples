//------------------------------------------------------------------------------
// File: SDLProgram.cpp
// Author: Chris Redwood
// Created: 2024-10-21
// License: MIT License
//------------------------------------------------------------------------------

#include "SDLProgram.hpp"

SDLProgram::~SDLProgram() {
    quit();
}

bool SDLProgram::init(int width, int height) {
    // TODO: 프로그램 초기화
    if(SDL_Init(SDL_INIT_EVENTS) < 0) {
        // TODO: 초기화 시 문제 해결
        SDL_assert(false);
        return false;
    }

    m_window = SDL_CreateWindow("SDL-Examples", 0, 0, width, height, SDL_WINDOW_RESIZABLE);
    if(m_window == nullptr) {
        SDL_assert(false);
        return false;
    }

    m_renderer = std::make_unique<SDLRenderer>(m_window);

    return true;
}

void SDLProgram::quit() {
    // TODO: 프로그램 종료
    // 렌더러를 먼저 삭제
    m_renderer.release();

    SDL_DestroyWindow(m_window);

    SDL_Quit();
}

SDL_Window* SDLProgram::window() {
    return m_window;
}

SDL_Renderer* SDLProgram::nativeRenderer() {
    return m_renderer->native();
}

std::unique_ptr<SDLRenderer>& SDLProgram::renderer() {
    return m_renderer;
}

bool SDLProgram::neededQuit() const {
    return m_neededQuit;
}

void SDLProgram::handleEvent(SDL_Event* event) {
    switch(event->type) {
        case SDL_QUIT: {
            m_neededQuit = true;
        }
        default: {}
    }
}