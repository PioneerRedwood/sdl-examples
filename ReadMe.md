# SDL-Examples
SDL2를 사용한 간단한 예제

## Build on macOS
```shell
> cmake -G Xcode -B ./build . # build 아래에 CMakeLists.txt에 명시한대로 Xcode 프로젝트 생성
```

## Blending
- Image load (Format: TGA - RGBA 32비트, 압축 없이)
- Alpha Blend, Add, Multiply 구현 https://wiki.libsdl.org/SDL2/SDL_BlendMode 
