# Set the name of the project and target:
SET(TARGET "IMtrajectoryModel")

FILE(GLOB_RECURSE TARGET_SRC  "src/*.cpp")

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)

PROJECT(${TARGET})

ADD_EXECUTABLE(${TARGET} ${TARGET_SRC})
