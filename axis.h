#define GLFW_INCLUDE_NONE
#include "glad.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
class Axis{
    public:
    Axis();
    void render(glm::mat4 projection, glm::mat4 view, glm::mat4 model);

    private:
    GLuint VBO;
    GLuint VAO;
    int shaderProgram;

    const char *axisVertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 color;\n"
    "out vec3 Color;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 view;\n"
    "uniform mat4 model;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"
    "   Color = color;\n"
    "}\0";
    const char *axisFragmentShaderSource = "#version 330 core\n"
    "in vec3 Color;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(Color, 1.0f);\n"
    "}\n\0";

    GLfloat vertices[36] = {
        -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, -3.0f, 0.0f, 0.0f, 1.0f
    };

};