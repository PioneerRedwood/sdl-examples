//------------------------------------------------------------------------------
// File: TGALoader.hpp
// Author: Chris Redwood
// Created: 2024-10-21
// License: MIT License
//------------------------------------------------------------------------------

#pragma once

#include <memory>
#include "TGA.hpp"

/**
 * TGA 확장자 파일 로더
 */
class TGALoader {
public:
    static std::unique_ptr<TGA*> readFromFilepath(const char*);

};