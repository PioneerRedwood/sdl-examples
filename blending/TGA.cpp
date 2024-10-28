//------------------------------------------------------------------------------
// File: TGA.cpp
// Author: Chris Redwood
// Created: 2024-10-21
// License: MIT License
//------------------------------------------------------------------------------

#include <cstdio>
#include <functional>
#include "TGA.hpp"

TGA::TGA() {}

TGA::~TGA() {
    if(m_pixel_data != nullptr) {
        delete[] m_pixel_data;
    }
    
    if(m_texture != nullptr) {
        SDL_DestroyTexture(m_texture);
    }
}

const TGAHeader* TGA::header() const {
    return (TGAHeader const*)&m_header;
}

const RGBA* TGA::pixelData() const {
    return m_pixel_data;
}

SDL_Texture const* TGA::sdlTexture() const {
    return m_texture;
}

bool TGA::readFromFile(const char* filepath) {
    FILE* fp = fopen(filepath, "rb");
    if(fp == nullptr) {
        // 파일 읽기 실패
        return false;
    }

    // Read header
    size_t read = fread(&m_header, sizeof(TGAHeader), 1, fp);
    if(read == 0) {
        // 파일 읽기 실패
        return false;
    }
    
    m_pixel_data = new RGBA[m_header.width * m_header.height];

    // Read pixel data
    const size_t imageSize = m_header.width * m_header.height * (m_header.pixel_depth / 8);
    read = fread(m_pixel_data, imageSize, 1, fp);
    if(read == 0) {
        return false;
    }

    fclose(fp);

    return true;
}

bool TGA::createTexture(SDL_Renderer *renderer) {
    m_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGRA32, 
                                  SDL_TEXTUREACCESS_STATIC, m_header.width, m_header.height);
    const int pitch = m_header.width * (m_header.pixel_depth / 8);
    if(SDL_UpdateTexture(m_texture, nullptr, m_pixel_data, pitch) != 0) {
        SDL_assert(false);
        return false;
    }
    
    // 만약 SDL_Texture로만 렌더링한다면 메모리 해제, 그렇지 않으면 메모리를 그대로 둬도 됨
    //delete[] m_pixel_data;
    //m_pixel_data = nullptr;
    
    return true;
}
