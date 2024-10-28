//------------------------------------------------------------------------------
// File: SDLRenderer.cpp
// Author: Chris Redwood
// Created: 2024-10-21
// License: MIT License
//------------------------------------------------------------------------------

#include "SDLRenderer.hpp"

#include <iostream>  // ste::cout long
#include <algorithm>

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

unsigned logCount = 0;

/// Alpha blend
/// dstRGB = (srcRGB * srcA) + (dstRGB * (1-srcA))
/// dstA = srcA + (dstA * (1-srcA))
RGBA alpha(const RGBA& src, const RGBA& dst) {
  RGBA c = {0};
  RGBAf s = {
    (float)src.r / 255.0f, (float)src.g / 255.0f, (float)src.b / 255.0f, (float)src.a / 255.0f
  };
  RGBAf d = {
    (float)dst.r / 255.0f, (float)dst.g / 255.0f, (float)dst.b / 255.0f, (float)dst.a / 255.0f
  };

  // src와 dst의 알파 값 미리 계산
  float invSrcAlpha = 1.0f - s.a;

  c.r = (uint8_t)(std::min((s.r * s.a + d.r * invSrcAlpha) * 255.0f, 255.0f));
  c.g = (uint8_t)(std::min((s.g * s.a + d.g * invSrcAlpha) * 255.0f, 255.0f));
  c.b = (uint8_t)(std::min((s.b * s.a + d.b * invSrcAlpha) * 255.0f, 255.0f));
  c.a = (uint8_t)(std::min((s.a + (d.a * invSrcAlpha)) * 255.0f, 255.0f));

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
  RGBAf s = {
    (float)src.r / 255.0f, (float)src.g / 255.0f, (float)src.b / 255.0f, (float)src.a / 255.0f
  };
  RGBAf d = {
    (float)dst.r / 255.0f, (float)dst.g / 255.0f, (float)dst.b / 255.0f, (float)dst.a / 255.0f
  };

  c.r = (uint8_t)(std::min(((s.r * s.a) + d.r) * 255.0f, 255.0f));
  c.g = (uint8_t)(std::min(((s.g * s.a) + d.g) * 255.0f, 255.0f));
  c.b = (uint8_t)(std::min(((s.b * s.a) + d.b) * 255.0f, 255.0f));
  c.a = dst.a;

  return c;
}

/// color multiply
/// dstRGB = (srcRGB * dstRGB) + (dstRGB * (1 - srcA))
/// dstA = dstA
RGBA multiply(const RGBA& src, const RGBA& dst) {
  RGBA c = { 0 };

  // src의 알파 값과 (1 - srcAlpha) 미리 계산
  float srcAlpha = src.a / 255.0f;
  float invSrcAlpha = 1.0f - srcAlpha;

  RGBAf s = {
    (float)src.r / 255.0f, (float)src.g / 255.0f, (float)src.b / 255.0f, (float)src.a / 255.0f
  };
  RGBAf d = {
    (float)dst.r / 255.0f, (float)dst.g / 255.0f, (float)dst.b / 255.0f, (float)dst.a / 255.0f
  };

  // RGB 채널에 대한 color multiply 연산 및 255로 제한
  c.r = (uint8_t)(std::min(((s.r * d.r) + (d.r * invSrcAlpha)) * 255.0f, 255.0f));
  c.g = (uint8_t)(std::min(((s.g * d.g) + (d.g * invSrcAlpha)) * 255.0f, 255.0f));
  c.b = (uint8_t)(std::min(((s.b * d.b) + (d.b * invSrcAlpha)) * 255.0f, 255.0f));

  // 알파 값은 변경하지 않음
  c.a = dst.a;

  return c;
}

}  // namespace blend

void SDLRenderer::drawTGA(std::unique_ptr<TGA>& tga, int x, int y) {
  SDL_Rect rect = {0};
  rect.x = x, rect.y = y, rect.w = tga->header()->width,
  rect.h = tga->header()->height;

  if (m_currentBlendMode == SDL_BLENDMODE_NONE) {
    SDL_RenderCopy(m_renderer, const_cast<SDL_Texture*>(tga->sdlTexture()),
      nullptr, &rect);
    return;
  }

  if (tga->pixelData() == nullptr) {
    std::cout << "TGA does not have any available pixels \n";
    return;
  }

  RGBA* pixels = new RGBA[tga->header()->width * tga->header()->height];
  if (pixels == nullptr) {
    return;
  }
  const int pitch = tga->header()->width * sizeof(RGBA);

  if (SDL_RenderReadPixels(m_renderer, &rect, SDL_PIXELFORMAT_BGRA32, pixels, pitch) != 0) {
    std::cout << "readPixels failed " << SDL_GetError() << std::endl;
    return;
  }
  RGBA* tgaPixels = const_cast<RGBA*>(tga->pixelData());
  for (int h = 0; h < tga->header()->height; ++h) {
    for (int w = 0; w < tga->header()->width; ++w) {
      int offset = h * tga->header()->width + w;
      // TODO: Do the math here
      RGBA blended = { 0 };
      switch (m_currentBlendMode) {
      case SDL_BLENDMODE_BLEND: {
        blended = blend::alpha(tgaPixels[offset], pixels[offset]);
        break;
      }
      case SDL_BLENDMODE_ADD: {
        blended = blend::additive(tgaPixels[offset], pixels[offset]);
        break;
      }
      case SDL_BLENDMODE_MUL: {
        blended = blend::multiply(tgaPixels[offset], pixels[offset]);
        break;
      }
      default: {
        blended = tgaPixels[offset];
        break;
      }
      }

      SDL_SetRenderDrawColor(m_renderer, blended.b, blended.g, blended.r,
        blended.a);
      SDL_RenderDrawPoint(m_renderer, rect.x + w, rect.y + h);
    }
  }
  delete[] pixels;
}

void SDLRenderer::present() { SDL_RenderPresent(m_renderer); }

void SDLRenderer::flush() { SDL_RenderFlush(m_renderer); }
