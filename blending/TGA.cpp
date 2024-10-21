//------------------------------------------------------------------------------
// File: TGA.cpp
// Author: Chris Redwood
// Created: 2024-10-21
// License: MIT License
//------------------------------------------------------------------------------

#include "TGA.hpp"
#include <cstdio>

TGA::TGA() {}

TGA::~TGA() {
    if(m_pixel_data) {
        delete[] m_pixel_data;
    }
}

bool TGA::readFromFile(const char* filepath) {
    FILE* fp = nullptr;
    errno_t err = fopen_s(&fp, filepath, "rb");
    if(err != 0) {
        // 파일 읽기 실패
        return false;
    }

    // Read header
    err = fread_s(&m_header, sizeof(TGAHeader), sizeof(TGAHeader), 1, fp);
    if(err == 0) {
        // 파일 읽기 실패
        return false;
    }
    m_pixel_data = new RGBA[m_header.width * m_header.height];

    // Read pixel data
    err = fread_s(m_pixel_data, 
        sizeof(RGBA) * m_header.width * m_header.height, 
        sizeof(RGBA) * m_header.width * m_header.height, 1, fp);
    if(err == 0) {
        return false;
    }

    fclose(fp);

    return true;
}
