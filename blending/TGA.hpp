//------------------------------------------------------------------------------
// File: TGA.hpp
// Author: Chris Redwood
// Created: 2024-10-21
// License: MIT License
//------------------------------------------------------------------------------

#pragma once

#include <cstdint>
#include <SDL.h>
#include "RGBA.hpp"

/**
 * TGA (Truevision Graphics Adapter), TARGA (Truevision Advanced Raster Graphics Adapter)
 * https://en.wikipedia.org/wiki/Truevision_TGA
 */

#pragma pack(push, 1)
struct TGAHeader {
    // Image ID Length (field 1)
    uint8_t id_length;

    // Color Map Type (field 2)
    uint8_t color_map_type;

    // Image Type (field 3)
    uint8_t image_type;
    
    // Color Map Specification (field 4)
    uint16_t first_entry_index;
    uint16_t color_map_length;
    uint8_t color_map_entry_size;

    // Image Specification (field 5)
    uint16_t x_origin;
    uint16_t y_origin;
    uint16_t width;
    uint16_t height;
    uint8_t pixel_depth; // bits per pixel
    uint8_t image_descriptor;

    // Image and Color Map Data
    // Image ID
    // Color map data
};

// Not used 
struct TGAFooter {
    uint32_t extension_offset;
    uint32_t developer_area_offset;
    uint16_t signature;
    uint8_t last_period;
    uint8_t nul;
};
#pragma pack(pop)

class TGA {
public:
    TGA();

    ~TGA();
    
    const TGAHeader* header() const;
    
    const RGBA* pixelData() const;
    
    SDL_Texture const* sdlTexture() const;

    bool readFromFile(const char* filepath);
    
    bool createTexture(SDL_Renderer* renderer);

private:
    TGAHeader m_header;

    RGBA* m_pixel_data = nullptr;
    
    SDL_Texture* m_texture = nullptr;
};
