cmake_minimum_required(VERSION 3.19)

# project name
project(sdl-examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)
set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "${CXX_FLAGS}")
set(CMAKE_PREFIX_PATH "lib/Windows/SDL2/lib/x86" "lib/Windows/SDL2/lib/x64")

# header files
set(HEADER_FILES
    blending/RGBA.hpp
    blending/TGA.hpp
    core/SDLRenderer.hpp
    core/SDLProgram.hpp
)

# source files
set(SORUCES
    blending/TGA.cpp
    core/SDLRenderer.cpp
    core/SDLProgram.cpp
    blending/main.cpp
)

# Create exec file
add_executable(sdl-examples
    ${HEADER_FILES}
    ${SORUCES}
)

# Settings for platform
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Windows specific settings (Visual Studio)
    message(STATUS "Configuring for Windows (Visual Studio)")

    # Windows include path
    include_directories(
        "lib/SDL2/Windows/include"
        "blending"
        "core"
    )

    # Windows link path
    link_directories(
        "lib/SDL2/Windows/lib/x86"
        "lib/SDL2/Windows/lib/x64"
    )

    target_link_libraries(sdl-examples
        SDL2
        SDL2main
    )

elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # macOS specific settings (Xcode)
    message(STATUS "Configuring for macOS (Xcode)")

    # macOS include path
    include_directories(
        "lib/SDL2/macOS/include"
        "blending"
        "core"
    )

    link_directories(
        "lib/SDL2/macOS/lib/arm64"
        "lib/SDL2/macOS/lib/x86_64"
    )

    # Find macOS frameworks
    find_library(CORE_FOUNDATION CoreFoundation REQUIRED)
    find_library(FOUNDATION Foundation REQUIRED)
    find_library(CORE_AUDIO CoreAudio REQUIRED)
    find_library(CORE_AUDIO_KIT CoreAudioKit REQUIRED)
    find_library(IO_KIT IOKit REQUIRED)
    find_library(CORE_GRAPHICS CoreGraphics REQUIRED)
    find_library(METAL_KIT MetalKit REQUIRED)
    find_library(AUDIO_TOOLBOX AudioToolbox REQUIRED)
    find_library(CORE_HAPTICS CoreHaptics REQUIRED)
    find_library(CORE_VIDEO CoreVideo REQUIRED)
    find_library(FORCE_FEEDBACK ForceFeedback REQUIRED)
    find_library(CARBON Carbon REQUIRED)
    find_library(GAME_CONTROLLER GameController REQUIRED)  # GameController 추가
    find_library(METAL Metal REQUIRED)  # Metal 추가
    find_library(APPKIT AppKit REQUIRED)  # AppKit 추가

    # x86_64
    add_library(lib-sdl2 STATIC IMPORTED)
    set_target_properties(lib-sdl2
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/lib/SDL2/macOS/lib/x86_64/libSDL2.a)

    add_library(lib-sdl2main STATIC IMPORTED)
    set_target_properties(lib-sdl2main
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/lib/SDL2/macOS/lib/x86_64/libSDL2main.a)

    target_link_libraries(sdl-examples
        lib-sdl2
        lib-sdl2main
        ${SDL2_LIBRARIES}
        ${CORE_FOUNDATION}
        ${FOUNDATION}
        ${CORE_AUDIO}
        ${CORE_AUDIO_KIT}
        ${IO_KIT}
        ${CORE_GRAPHICS}
        ${METAL_KIT}
        ${AUDIO_TOOLBOX}
        ${CORE_HAPTICS}
        ${CORE_VIDEO}  # CoreVideo 링크
        ${FORCE_FEEDBACK}  # ForceFeedback 링크
        ${CARBON}  # Carbon 링크
        ${GAME_CONTROLLER}  # GameController 링크
        ${METAL}  # Metal 링크
        ${APPKIT}  # AppKit 링크        
    )
endif()

# SDL2 DLL 복사
if(WIN32)
    set(SDL2_DLL "${CMAKE_CURRENT_SOURCE_DIR}/lib/Windows/SDL2/lib/x86/SDL2.dll")
    add_custom_command(TARGET sdl-examples POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_DLL}" $<TARGET_FILE_DIR:sdl-examples>)
endif()
