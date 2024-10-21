
# the name of the target operating system
# set(CMAKE_SYSTEM_NAME Windows)
#
# # which compilers to use for C and C++
# set(CMAKE_C_COMPILER   clang)
# set(CMAKE_CXX_COMPILER clang++)


set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)

cmake_minimum_required(VERSION 3.22.1)

project(GameTime VERSION 0.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)


add_executable(GameTime 
                        src/main.cpp
                        )
target_include_directories(GameTime PRIVATE include)

if(MSVC)
    message("BUILDING: MSVC")
    target_compile_options(GameTime PRIVATE /W4 /WX)
elseif(WIN32)
    message("BUILDING: win32")
    target_compile_options(GameTime PRIVATE -Wall -Wextra -Wpedantic)
else()
    message("BUILDING: else")
    target_compile_options(GameTime PRIVATE -g -Wall -Wextra -Wpedantic)
endif()



add_subdirectory(lib/glfw-3.4)
SET(GLM_TEST_ENABLE OFF CACHE BOOL "GLM Build unit tests")
find_package(Vulkan REQUIRED)

target_link_libraries(GameTime glfw Vulkan::Vulkan)
