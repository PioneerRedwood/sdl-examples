//------------------------------------------------------------------------------
// File: SDLRenderer.cpp
// Author: Chris Redwood
// Created: 2024-10-21
// License: MIT License
//------------------------------------------------------------------------------

#include "SDLRenderer.hpp"

#include <iostream>  // ste::cout long

SDLRenderer::SDLRenderer(SDL_Window* window) {
  // TODO: 과연 생성자에 결과를 확인해야 할수도 있는 인스턴스를 초기화하는 것이
  // 좋을까?
  m_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (m_renderer == nullptr) {
    SDL_assert(false);
  }
}

SDLRenderer::~SDLRenderer() { SDL_DestroyRenderer(m_renderer); }

SDL_Renderer* SDLRenderer::native() { return m_renderer; }

void SDLRenderer::clear() { SDL_RenderClear(m_renderer); }

void SDLRenderer::enableBlending(SDL_BlendMode blendMode) {
  m_currentBlendMode = blendMode;
}

void SDLRenderer::disableBlending() { m_currentBlendMode = SDL_BLENDMODE_NONE; }

namespace blend {

struct RGBAf {
  float r, g, b, a;
};

char logBuf[512];

unsigned logCount = 16;

/// Alpha blend
/// dstRGB = (srcRGB * srcA) + (dstRGB * (1-srcA))
/// dstA = srcA + (dstA * (1-srcA))
RGBA alpha(const RGBA& src, const RGBA& dst) {
  RGBA c = {0};
  c.r = (uint8_t)(((((float)src.r / 255) * ((float)src.a / 255)) + (((float)dst.r / 255) * (1.0f - ((float)src.a / 255)))) * 255);
  c.g = (uint8_t)(((((float)src.g / 255) * ((float)src.a / 255)) + (((float)dst.g / 255) * (1.0f - ((float)src.a / 255)))) * 255);
  c.b = (uint8_t)(((((float)src.b / 255) * ((float)src.a / 255)) + (((float)dst.b / 255) * (1.0f - ((float)src.a / 255)))) * 255);
  c.a = (uint8_t)((((float)src.a / 255) + (((float)dst.a / 255) * (1.0 - ((float)src.a / 255)))) * 255);

  if(logCount > 0) {
    snprintf(logBuf, 512, "src: %.3f %.3f %.3f %.3f & dst %.3f %.3f %.3f %.3f = %.3f %.3f %.3f %.3f \n",
      ((float)src.r / 255), ((float)src.g / 255), ((float)src.b / 255), ((float)src.a / 255),
      ((float)dst.r / 255), ((float)dst.g / 255), ((float)dst.b / 255), ((float)dst.a / 255),
      ((float)c.r / 255), ((float)c.g / 255), ((float)c.b / 255), ((float)c.a / 255));
    printf("%.2d %s", logCount, logBuf);
    logCount--;
  }
  return c;
}

/// additive blending
/// dstRGB = (srcRGB * srcA) + dstRGB
/// dstA = dstA
RGBA additive(const RGBA& src, const RGBA& dst) {
  RGBA c = {0};
  c.r = (uint8_t)( (((float)src.r / 255) * ((float)src.a / 255) + ((float)dst.r / 255)) * 255 );
  c.g = (uint8_t)( (((float)src.g / 255) * ((float)src.a / 255) + ((float)dst.g / 255)) * 255 );
  c.b = (uint8_t)( (((float)src.b / 255) * ((float)src.a / 255) + ((float)dst.b / 255)) * 255 );
  c.a = dst.a;
    
  if(logCount > 0) {
    snprintf(logBuf, 512, "src: %.3f %.3f %.3f %.3f & dst %.3f %.3f %.3f %.3f = %.3f %.3f %.3f %.3f \n",
      ((float)src.r / 255), ((float)src.g / 255), ((float)src.b / 255), ((float)src.a / 255),
      ((float)dst.r / 255), ((float)dst.g / 255), ((float)dst.b / 255), ((float)dst.a / 255),
      ((float)c.r / 255), ((float)c.g / 255), ((float)c.b / 255), ((float)c.a / 255));
    printf("%.2d %s", logCount, logBuf);
    logCount--;
  }

  return c;
}

/// color multiply
/// dstRGB = (srcRGB * dstRGB) + (dstRGB * (1-srcA))
/// dstA = dstA
RGBA multiply(const RGBA& src, const RGBA& dst) {
  RGBA c = {0};
  c.r = (uint8_t)( (((float)src.r / 255) * ((float)dst.r / 255) + (((float)dst.r / 255) * (1.0f - ((float)src.r / 255)))) * 255 );
  c.g = (uint8_t)( (((float)src.g / 255) * ((float)dst.g / 255) + (((float)dst.g / 255) * (1.0f - ((float)src.g / 255)))) * 255 );
  c.b = (uint8_t)( (((float)src.b / 255) * ((float)dst.b / 255) + (((float)dst.b / 255) * (1.0f - ((float)src.b / 255)))) * 255 );
  c.a = dst.a;
  return c;
}

}  // namespace blend

void SDLRenderer::drawTGA(std::unique_ptr<TGA>& tga, int x, int y) {
  SDL_Rect rect = {0};
  rect.x = x, rect.y = y, rect.w = tga->header()->width,
  rect.h = tga->header()->height;

  switch (m_currentBlendMode) {
    case SDL_BLENDMODE_BLEND: {
      drawWithBlending(tga, &rect);
      break;
    }
    case SDL_BLENDMODE_ADD: {
      drawWithBlending(tga, &rect);
      break;
    }
    case SDL_BLENDMODE_MUL: {
      drawWithBlending(tga, &rect);
      break;
    }
    default: {
      SDL_RenderCopy(m_renderer, const_cast<SDL_Texture*>(tga->sdlTexture()),
                     nullptr, &rect);
      break;
    }
  }
}

void SDLRenderer::drawWithBlending(std::unique_ptr<TGA>& tga, const SDL_Rect* rect) {
  if (tga->pixelData() == nullptr) {
    std::cout << "TGA does not have any available pixels \n";
    return;
  }
  auto blendFunc = [](SDL_BlendMode blendMode, const RGBA& s, const RGBA& d)->RGBA {
    switch(blendMode) {
      case SDL_BLENDMODE_BLEND: {
        return blend::alpha(s, d);
        break;
      }
      case SDL_BLENDMODE_ADD: {
        return blend::additive(s, d);
        break;
      }
      case SDL_BLENDMODE_MUL: {
        return blend::multiply(s, d);
        break;
      }
      default: return s;
    }
  };

  // TODO: 이 화면에서 가져오는 픽셀 데이터를 매번 할당, 해제해야 하는가? 다른 방법은?
  RGBA* pixels = new RGBA[tga->header()->width * tga->header()->height];
  const int pitch = tga->header()->width * sizeof(RGBA);

  if(SDL_RenderReadPixels(m_renderer, rect, SDL_PIXELFORMAT_BGRA32, pixels, pitch) != 0) {
    std::cout << "readPixels failed " << SDL_GetError() << std::endl;
    return;
  }
  RGBA* tgaPixels = const_cast<RGBA*>(tga->pixelData());
  for (int h = 0; h < tga->header()->height; ++h) {
    for (int w = 0; w < tga->header()->width; ++w) {
      int offset = h * tga->header()->width + w;
      // TODO: Do the math here
      RGBA blended = blendFunc(m_currentBlendMode, tgaPixels[offset], pixels[offset]);

      SDL_SetRenderDrawColor(m_renderer, blended.b, blended.g, blended.r,
                              blended.a);
      SDL_RenderDrawPoint(m_renderer, w, h);
    }
  }
  delete[] pixels;
}

void SDLRenderer::present() { SDL_RenderPresent(m_renderer); }

void SDLRenderer::flush() { SDL_RenderFlush(m_renderer); }
