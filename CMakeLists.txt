cmake_minimum_required(VERSION 3.10)

project(SDLTetris VERSION 0.1 LANGUAGES CXX)

set(PROJECT_SOURCES
    SDLTetris.cpp
    Tetromino.cpp Tetromino.h
    TetrisConst.h
    IGameMode.h
    StandByMode.h
    StandByMode.cpp
    PlayMode.h
    PlayMode.cpp
    HighScoresMode.h
    HighScoresMode.cpp
    GameOverMode.h
    GameOverMode.cpp
    Game.h
    Game.cpp

)

add_executable(SDLTetris ${PROJECT_SOURCES})

set(SDL2_STATIC_LIBRARIES FALSE)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(fmt REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

if(MINGW)
    message("--> MSYS " $ENV{USER})
    add_compile_definitions(_MSYS)
endif()


if(LINUX)
    message("--> LINUX " $ENV{USER})
    # add_compile_definitions(_MSYS )
endif()

target_compile_features(SDLTetris PUBLIC cxx_std_20)

target_compile_definitions(SDLTetris PRIVATE SDL2_SHARED)

if(MINGW)
  target_link_libraries(SDLTetris mingw32 SDL2main SDL2 SDL2_ttf SDL2_mixer SDL2_image fmt)
endif()

if(LINUX)
    message("--> LINUX " $ENV{USER})
    target_link_libraries(SDLTetris SDL2main SDL2 SDL2_ttf SDL2_mixer SDL2_image fmt)
endif()
