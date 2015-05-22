CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(games)

# C++11, all warnings
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

# Brickbreaker
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/01-brickbreaker/src")
FILE(GLOB_RECURSE BRICKBREAKER_SOURCES "${CMAKE_SOURCE_DIR}/01-brickbreaker/src/*.cpp")
ADD_EXECUTABLE(brickbreaker ${BRICKBREAKER_SOURCES})

# Pong
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/02-pong/src")
FILE(GLOB_RECURSE PONG_SOURCES "${CMAKE_SOURCE_DIR}/02-pong/src/*.cpp")
ADD_EXECUTABLE(pong ${PONG_SOURCES})

# Game of Life
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/03-game-of-life/src")
FILE(GLOB_RECURSE LIFE_SOURCES "${CMAKE_SOURCE_DIR}/03-game-of-life/src/*.cpp")
ADD_EXECUTABLE(life ${LIFE_SOURCES})

# SFML
SET(SFML_ROOT "${CMAKE_SOURCE_DIR}/libs/SFML-2.2")
SET(SFML_LIB_DIR "${SFML_ROOT}/build/lib")
INCLUDE_DIRECTORIES("${SFML_ROOT}/include")
ADD_SUBDIRECTORY("${SFML_ROOT}")

# Link
TARGET_LINK_LIBRARIES(brickbreaker sfml-graphics
                           		   sfml-system
                           		   sfml-window
)
TARGET_LINK_LIBRARIES(pong sfml-graphics
                           sfml-system
                           sfml-window
)
TARGET_LINK_LIBRARIES(life sfml-graphics
                           sfml-system
                           sfml-window
)