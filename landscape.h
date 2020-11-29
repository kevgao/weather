#define GLFW_INCLUDE_NONE
#include "glad.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

class Landscape{
    public:
    Landscape();
    void render(glm::mat4 projection, glm::mat4 view, glm::mat4 model);

    private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint shaderProgram;
    unsigned int texture;
    void skybox();
    void land();

    const char* landscapeVertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "layout (location = 1) in vec3 dnorm;\n"
        "layout (location = 2) in vec2 dtexCoord;\n"
        "out vec3 norm;\n"
        "out vec2 texCoord;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "void main(){\n"
        "   gl_Position =  projection * view * model * vec4(position, 1.0f);\n"
        "   norm = dnorm;\n"
        "   texCoord = dtexCoord;\n"
        "}\0";

    const char* landscapeFragmentShaderSource = "#version 330 core\n"
        "in vec3 norm;\n"
        "in vec2 texCoord;\n"
        "out vec4 fragColor;\n"
        "uniform sampler2D ourTexture;\n"
        "void main(){\n"
        "   fragColor = texture(ourTexture, texCoord);\n"
        "}\0";

    GLfloat vertices[32] = {
        0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };
    GLuint indices[6] = {
        0, 1, 3,
        1, 2, 3
    };

};
