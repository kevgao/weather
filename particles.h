#define GLFW_INCLUDE_NONE
#include "glad.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

typedef struct{
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 color;
    float age;
    float life;
    float size;
}Particle;

class ParticleSystem{
    public:
    ParticleSystem();
    void init();
    void aging();
    void simulate();
    void render();

    private:
    GLint particleCount;
    int shaderProgram;
    GLuint VAO;
    GLuint vertexVBO;
    GLuint particleVBO;

    const char *particleVertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 position;\n"
    "layout (location = 2) in vec3 velocity;\n"
    "layout (location = 3) in vec4 color;\n"
    "layout (location = 4) in vec1 age;\n"
    "layout (location = 5) in vec1 life;\n"
    "layout (location = 6) in vec1 size;\n"
    "out vec3 Color;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 view;\n"
    "uniform mat4 model;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(position, 1.0f);\n"
    "   Color = color;\n"
    "}\0";
    const char *particleFragmentShaderSource = "#version 330 core\n"
    "in vec3 Color;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(Color, 1.0f);\n"
    "}\n\0";

    GLfloat vertices[16] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f
    };
    Particle particles[100];

    Particle generateParticle();
    

};