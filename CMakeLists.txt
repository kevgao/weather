cmake_minimum_required(VERSION 3.6)

project(weathersystem)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# glfw 
find_package(glfw3 3.3 REQUIRED)

# glad
add_library(glad STATIC
    lib/glad/glad.c
)

# imgui
add_library(imgui STATIC
    lib/imgui/imgui.cpp
    lib/imgui/imgui_draw.cpp
    lib/imgui/imgui_impl_glfw.cpp
    lib/imgui/imgui_impl_opengl3.cpp
    lib/imgui/imgui_widgets.cpp
    lib/imgui/imgui.cpp
)
target_include_directories(imgui
    PRIVATE
        ${PROJECT_SOURCE_DIR}/lib
)
target_link_libraries(imgui glfw glad)


# glm
add_library(glm INTERFACE) #using INTERFACE to generate header file only libraries
target_include_directories(glm INTERFACE lib/glm/)

# stbi_image
add_library(stbi_image INTERFACE)
target_include_directories(stbi_image INTERFACE lib/stbi/)

# main program
set(SOURCES
    src/axis.cpp
    src/grass.cpp
    src/project.cpp
    src/landscape.cpp
    src/particles.cpp
    src/project.cpp
    src/skybox.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} 
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/lib
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        glfw
        imgui
        glad
        glm
        stbi_image
)

# Resources
file(COPY image DESTINATION ${CMAKE_BINARY_DIR})