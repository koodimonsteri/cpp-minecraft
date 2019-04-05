cmake_minimum_required(VERSION 3.5)
project(cpp-mc)
set(CMAKE_CXX_STANDARD 17) 

# GLFW build oprions
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_VULKAN_STATIC OFF CACHE BOOL "" FORCE)

if (APPLE)
set(GLFW_USE_RETINA ON CACHE BOOL "" FORCE)
endif (APPLE)

# GLM build options
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)

add_subdirectory(3rdparty/glfw)
add_subdirectory(3rdparty/glm)
add_subdirectory(3rdparty/glad)

include_directories(include 3rdparty/stb/include 3rdparty/fastnoise)
file(GLOB_RECURSE SOURCES "src/*" "3rdparty/fastnoise/FastNoise.cpp")

add_executable(cpp-mc ${SOURCES})

target_compile_options(cpp-mc PUBLIC -DGLM_ENABLE_EXPERIMENTAL -DSTB_IMAGE_IMPLEMENTATION)
target_link_libraries(cpp-mc glm glfw glad)