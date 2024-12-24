cmake_minimum_required(VERSION 3.5)


    project(SDLTetris VERSION 0.1 LANGUAGES CXX)

    set(PROJECT_SOURCES
                SDLTetris.cpp
                Tetromino.cpp Tetromino.h
                TetrisConst.h
 	)

    add_executable(SDLTetris ${PROJECT_SOURCES})

    set(SDL2_STATIC_LIBRARIES FALSE)
    find_package(SDL2_image REQUIRED)

    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

    if (MSYS)
        message("--> MSYS " $ENV{USER})
        add_compile_definitions(_MSYS )
    endif()

    if (LINUX)
        message("--> LINUX " $ENV{USER})
        #add_compile_definitions(_MSYS )
    endif()

    target_compile_features(SDLTetris PUBLIC cxx_std_20)

    target_compile_definitions(SDLTetris PRIVATE SDL2_SHARED)

    target_link_libraries(SDLTetris SDL2 SDL2main SDL2_ttf SDL2_mixer SDL2_image fmt)
