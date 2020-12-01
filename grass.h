#define GLFW_INCLUDE_NONE
#include "glad.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

#define MAX_BLADE_COUNT 10000

typedef struct{
    glm::vec3 position;
    float distortion;
}Blade;

class Grass{
    public:
    Grass();
    void update();
    void render(glm::mat4 projection, glm::mat4 view, glm::mat4 model);

    private:
    GLint bladeCount;
    int shaderProgram;
    GLuint VAO;
    GLuint vertexVBO;
    GLuint bladeVBO;
    GLuint texture;

    const char *bladeVertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 position;\n"
    //"layout (location = 2) in float distortion;\n"
    "out vec2 textCoord;\n"
    //"out float visibility;\n"
    //"float density = 0.3;\n"
    //"float gradient = 2.0;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 view;\n"
    "uniform mat4 model;\n"
    "void main()\n"
    "{\n"
    //"   vec4 positionRelativeToCam = view * model * vec4(0.05 * aPos + position, 1.0f);\n"
    //"   gl_Position = projection * positionRelativeToCam;\n"
    "   gl_Position = projection * view * model * vec4(0.05 * aPos + position, 1.0f);\n"
    "   textCoord = vec2(aPos.x, aPos.y);\n"
    //"   float distance = length(positionRelativeToCam.xyz);\n"
    //"   visibility = exp(-pow((distance*density),gradient);\n"
    //"   visibility = (10.0-distance)/10.0;\n"
    //"   visibility = clamp(visibility, 0.0, 1.0);\n"
    "}\0";
    const char *bladeFragmentShaderSource = "#version 330 core\n"
    "in vec2 textCoord;\n"
    //"in float visibility;\n"
    "out vec4 fragColor;\n"
    "uniform sampler2D tex;\n"
    "void main()\n"
    "{\n"
    //"   fragColor = vec4(Color);\n"
    //"   fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
    //"   vec4 texColor = texture(tex, textCoord);\n"
    "   fragColor = texture(tex, textCoord);\n"
    //"   if(fragColor.g < 0.3)discard;\n"
    //"   fragColor = mix(vec4(0.5, 0.5, 0.5, 1.0), fragColor, visibility);\n"
    //"   if(texColor.r < 0.3)discard;\n"
    //"   fragColor = vec4(1.0f, 1.0f, 1.0f, texColor.a);\n"
    //"   fragColor = texture(tex, gl_PointCoord);\n"

    "}\n\0";

    GLfloat vertices[18] = {
        0.0f,  1.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
    };
    Blade blades[MAX_BLADE_COUNT];

    Blade generateBlade();
    

};