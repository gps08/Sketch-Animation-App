cmake_minimum_required(VERSION 3.8)
project(sketch_animation)

set(CMAKE_CXX_STANDARD 11)
file(GLOB DEPS_SOURCES ext/glad/src/glad.c)
include_directories("ext/glfw/include" "ext/glad/include" "src" "ext/imgui" "ext/glm/")

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(ext/glfw)

file(GLOB IMGUI_SOURCE
        ext/imgui/*.cpp
        ext/imgui/*.h
        ext/glm/
        )

set(SOURCE_FILES main.cpp src/SketchWin.h src/ShadersUtil.h src/Canvas.h src/Doodle.h src/Object.h src/Rectangle.h src/Motion.h src/Node.h src/Arrow.h)

add_executable(sketch_animation ${SOURCE_FILES}  ${DEPS_SOURCES} ${IMGUI_SOURCE})
target_link_libraries(sketch_animation glfw ${GLFW_LIBRARIES} ${PLATFORM_LINK} )