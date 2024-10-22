cmake_minimum_required(VERSION 3.19)

# project name
project(sdl-examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE DEBUG)
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
    )

    target_link_libraries(sdl-examples
      "-framework CoreFoundation"
      "-framework Foundation"
      "-framework CoreAudioKit"
      "-framework IOKit"
      "-framework CoreGraphics"
      "-framework MetalKit"
    )

endif()

# link other libraries if needed
target_link_libraries(sdl-examples
    # other libraries
    SDL2
    SDL2main
    # ${MACOS_LIBRARIES}
)

# SDL2 DLL 복사
if(WIN32)
    set(SDL2_DLL "${CMAKE_CURRENT_SOURCE_DIR}/lib/Windows/SDL2/lib/x86/SDL2.dll")
    add_custom_command(TARGET sdl-examples POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_DLL}" $<TARGET_FILE_DIR:sdl-examples>)
endif()
