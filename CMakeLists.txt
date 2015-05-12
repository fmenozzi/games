CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(games)

# C++11, all warnings
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

# Add brickbreaker source
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/01-brickbreaker/src")
FILE(GLOB_RECURSE BRICKBREAKER_SOURCES "${CMAKE_SOURCE_DIR}/01-brickbreaker/src/*.cpp")
ADD_EXECUTABLE(brickbreaker ${BRICKBREAKER_SOURCES})

# SFML
SET(SFML_ROOT "${CMAKE_SOURCE_DIR}/libs/SFML-2.2")
SET(SFML_LIB_DIR "${SFML_ROOT}/build/lib")
INCLUDE_DIRECTORIES("${SFML_ROOT}/include")
ADD_SUBDIRECTORY("${SFML_ROOT}")
TARGET_LINK_LIBRARIES(brickbreaker sfml-audio
                                   sfml-graphics
                                   sfml-network
                                   sfml-system
                                   sfml-window
)
