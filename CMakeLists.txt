cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 17)
project(srend)

# SDL2
# =======
if(WIN32)

    #
    # MSYS MINGW
    #

    INCLUDE(FindPkgConfig)

    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2_IMG REQUIRED SDL2_image)
    pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
    pkg_check_modules(SDL2_MIX REQUIRED SDL2_mixer)


    link_directories (${SDL2_LIBRARY_DIRS}
            ${SDL2_IMG_LIBRARY_DIRS}
            ${SDL2_TTF_LIBRARY_DIRS}
            ${SDL2_MIX_LIBRARY_DIRS})
			
endif(WIN32)
if(UNIX)
    INCLUDE(FindPkgConfig)

    PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
    PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image >= 2.0.0)
    PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf >= 2.0.0)
    PKG_SEARCH_MODULE(SD2MIXER REQUIRED SDL2_mixer >= 2.0.0)

    set(SDL2_LIB -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lglfw -lGLEW)

    set(SDL2_INCLUDE ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS})
endif(UNIX)

# =======

# PROJECT-FILES
# =============
# set(SREND_FILES srend/core.cpp)
file(GLOB_RECURSE SREND_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE BOX2D ${PROJECT_SOURCE_DIR}/lib/box2d/src/*.cpp)
file(GLOB_RECURSE LIB_FILES ${PROJECT_SOURCE_DIR}/lib/*.cpp)

# Finalising
if(WIN32)
    set(FINAL_INCLUDE ${SDL2_INCLUDE_DIRS}
            ${SDL2_IMG_INCLUDE_DIRS}
            ${SDL2_TTF_INCLUDE_DIRS}
            ${SDL2_MIX_INCLUDE_DIRS} ${ENGINE_INCLUDE_DIR})

    set(FINAL_LINKER
            -lmingw32
            ${SDL2_LIBRARIES}
            ${SDL2_IMG_LIBRARIES}
            ${SDL2_TTF_LIBRARIES}
            ${SDL2_MIX_LIBRARIES}
            -lopengl32
            -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -lfreetype
    )

    link_directories (${SDL2_LIBRARY_DIRS}
            ${SDL2_IMG_LIBRARY_DIRS}
            ${SDL2_TTF_LIBRARY_DIRS}
            ${SDL2_MIX_LIBRARY_DIRS})
endif(WIN32)
if(UNIX)
    find_package(Freetype REQUIRED)
    SET(FINAL_INCLUDE ${SDL2_INCLUDE} ${FREETYPE_INCLUDE_DIRS})
    SET(FINAL_LINKER ${SDL2_LIB} ${FREETYPE_LIBRARIES})
endif(UNIX)
# =============


add_library(srend STATIC ${SREND_FILES} ${BOX2D} ${LIB_FILES})
target_include_directories(srend PUBLIC ${CMAKE_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/lib/box2d/src ${PROJECT_SOURCE_DIR}/lib ${FREETYPE_INCLUDE_DIR_ft2build})


if(UNIX)
    target_link_libraries(srend ${FINAL_LINKER} "-framework OpenGL")
else(UNIX)
    target_link_libraries(srend ${FINAL_LINKER})
endif(UNIX)