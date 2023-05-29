cmake_minimum_required(VERSION 3.0.0)
project(graphs VERSION 0.1.0)

include(CTest)
enable_testing()

set(SRC_LIST
    src/ArcGraph.cpp
    src/ListGraph.cpp
    src/MatrixGraph.cpp
    src/SetGraph.cpp
)

add_library(${PROJECT_NAME} ${SRC_LIST})
target_include_directories(${PROJECT_NAME} PUBLIC include)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
