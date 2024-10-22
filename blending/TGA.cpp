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
    if(read != sizeof(TGAHeader)) {
        // 파일 읽기 실패
        return false;
    }
    const size_t imageSize = m_header.width * m_header.height;
    m_pixel_data = new RGBA[imageSize];

    // Read pixel data
    read = fread(m_pixel_data, 
        sizeof(RGBA) * imageSize, 1, fp);
    if(read != (sizeof(RGBA) * imageSize)) {
        return false;
    }

    fclose(fp);

    return true;
}
