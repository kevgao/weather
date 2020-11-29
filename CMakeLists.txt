cmake_minimum_required(VERSION 3.6)

project(weathersystem)

include_directories("{CMAKE_SOURCE_DIR}/imgui")
include_directories("{CMAKE_SOURCE_DIR}/glm")
include_directories("{CMAKE_SOURCE_DIR}/image")

set(GLAD "glad.h" "glad.c")

list(APPEND MAIN_APP_SOURCES
    project.h
    project.cpp
    glad.c
    glad.h
    khrplatform.h
    imgui/imgui_impl_opengl3.h
    imgui/imgui_impl_opengl3.cpp
    imgui/imgui_impl_glfw.h
    imgui/imgui_impl_glfw.cpp
)

set(MAIN_APP_LIBRARIES)
    imgui
    glfw
)

list(APPEND MAIN_APP_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui"
)

set(NAME weathersystem)
add_executable(${NAME} ${MAIN_APP_SOURCES})
target_include_directories(${NAME} PUBLIC ${MAIN_APP_INCLUDE_DIRS})
target_link_libraries(${NAME} ${MAIN_APP_LIBRARIES})