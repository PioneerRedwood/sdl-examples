//------------------------------------------------------------------------------
// File: TGA.cpp
// Author: Chris Redwood
// Created: 2024-10-21
// License: MIT License
//------------------------------------------------------------------------------

#include <cstdio>
#include "TGA.hpp"

TGA::TGA() {}

TGA::~TGA() {
    if(m_pixel_data) {
        delete[] m_pixel_data;
    }
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
    const size_t imageSize = sizeof(RGBA) * m_header.width * m_header.height;
    read = fread(m_pixel_data, imageSize, 1, fp);
    if(read == 0) {
        return false;
    }

    fclose(fp);

    return true;
}

bool TGA::createTexture(SDL_Renderer *renderer) {
    m_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, m_header.width, m_header.height);
    void* pixels;
    int pitch;
    if(SDL_LockTexture(m_texture, nullptr, &pixels, &pitch) != 0) {
        memcpy(pixels, m_pixel_data, sizeof(RGBA) * m_header.width * m_header.height);
    } else {
        return false;
    }
    SDL_UnlockTexture(m_texture);
    delete[] m_pixel_data;
    m_pixel_data = nullptr;
    
    return true;
}
