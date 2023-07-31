cmake_minimum_required(VERSION 3.5)
project(trackTask VERSION 0.1.0 LANGUAGES C CXX)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

find_package(OpenCV REQUIRED)

# 查找 OpenMP 支持
find_package(OpenMP)

# 如果找到 OpenMP 库，添加编译选项
if(OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

include(CPack)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

include(CTest)
enable_testing()

file(GLOB_RECURSE SOURCES "source/*.cpp")
add_executable(trackTask ${SOURCES})

target_link_libraries(trackTask ${OpenCV_LIBS})