cmake_minimum_required(VERSION 3.19)

# project name
project(sdl-examples)

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

# Settings for platform
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Windows specific settings (Visual Studio)
    message(STATUS "Configuring for Windows (Visual Studio)")

    # Windows include path
    include_directories(
        "lib/Windows/SDL2/include"
        "blending"
        "core"
    )

    # Windows link path
    link_directories(
        "lib/Windows/SDL2/lib/x86"
        "lib/Windows/SDL2/lib/x64"
    )

elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # macOS specific settings (Xcode)
    message(STATUS "Configuring for macOS (Xcode)")

    # Create Xcode project
    set(CMAKE_GENERATOR "Xcode" CACHE STRING "" FORCE)

    # macOS include path
    include_directories(
        
    )

    link_directories(
        
    )

endif()

# Create exec file
add_executable(sdl-examples
    ${HEADER_FILES}
    ${SORUCES}
)

# link other libraries if needed
target_link_libraries(sdl-examples
    # other libraries
    SDL2
    SDL2main
)

# SDL2 DLL 복사
if(WIN32)
    set(SDL2_DLL "${CMAKE_CURRENT_SOURCE_DIR}/lib/Windows/SDL2/lib/x86/SDL2.dll")
    add_custom_command(TARGET sdl-examples POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_DLL}" $<TARGET_FILE_DIR:sdl-examples>)
endif()
