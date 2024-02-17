Not sure how to have it here properly but libraries used i did not write:
- json : https://github.com/nlohmann/json
- box2d : https://github.com/erincatto/box2d
- stb_image : http://nothings.org/stb

other required libraries that arent included with the project:
- SDL2
- SDL2_image
- SDL2_ttf
- SDL2_mixer
- OpenGl
- FreeType2

Currently only builds on my macbook. Didnt try anywhere else.


## What is this?
It's a small OpenGl based game engine i wrote. It's very minimal and only handles a few very basic things - loading files, drawing images & text, hierarchy, simple spritesheet based animation, tweens (there's only 1 tween atm and it's scale), events, random, input and physics (using box2d).
I included the basic shaders (it's inside shaders folder) for drawing stuff, but you'll have to add them to your project yourself. To start using this just include "core.h"
```cpp
#include <stdio.h>
#include <string>
#include <iostream>

#include "srend/core.h"
#include "game.h"


int main( int argc, char* args[] ) {

    bool launch = true;
    if ( !srend::init("SDL", 640, 480) ) {
        SYSLOG("Failed to initialize\n");
        launch = false;
    }

    auto physicsConfig = srend::PhysicsConfig();
    physicsConfig.gravity.y = -9.8f;

    srend::physics.init(physicsConfig);
    srend::physics.setDebugDrawEnabled(false);

    Configs::getInstance().init();

    auto game = Game();
    srend::loadScene(&game);


    if ( launch ) {
        srend::run();
    }

    srend::exit();

    return 0;
}
```

## Cmake
I'm not 100% sure about the Cmake usage, it's very primitive but it helps me build, so i'm using it. It most likely is not 100% correct.
- Example of how my own cmake looks like that's meant to build this in to an actual game:
```cmake
cmake_minimum_required(VERSION 3.6)
project(StarSurvivor)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-pie")
# set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++${CMAKE_CXX_STANDARD}")


add_subdirectory(srend)

# PROJECT-FILES
# =============
# set(SREND_FILES srend/core.cpp)
file(GLOB_RECURSE GAME_FILES ${PROJECT_SOURCE_DIR}/star_survivor/*.cpp)
set(SOURCE_FILES main.cpp)

# -PROJECT-RESOURCES
# Copies files (preserving file tree) from /res folder to debug /res folder.

set(PROJECT_BINARY_DIR ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE RES_FILES ${PROJECT_SOURCE_DIR}/res/*)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/res)
foreach(F ${RES_FILES})
    get_filename_component(q ${F} DIRECTORY)
    file(RELATIVE_PATH g ${PROJECT_SOURCE_DIR}/res ${q}  )
    if(g)
        file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/res/${g})
    endif(g)
    file(COPY ${F} DESTINATION ${PROJECT_BINARY_DIR}/res/${g})
endforeach()
# =============

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
add_executable(StarSurvivor ${SOURCE_FILES} ${GAME_FILES})

target_link_libraries(StarSurvivor PUBLIC srend)
```