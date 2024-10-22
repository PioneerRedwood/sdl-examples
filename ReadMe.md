# SDL-Examples
SDL2를 사용한 간단한 예제

## Build Instructions

### macOS
```shell
> sh scripts/build-macos.sh
```

### Windows
```shell
> cmake -B ./build .
```

## Blending
- Image load (Format: TGA - RGBA 32비트, 압축 없이)
- Alpha Blend, Add, Multiply 구현 https://wiki.libsdl.org/SDL2/SDL_BlendMode 
