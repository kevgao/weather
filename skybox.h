#define GLFW_INCLUDE_NONE
#include "glad.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <string>

class CubemapTexture{
    public:
    CubemapTexture(
        /*
        const std::string& PosXFilename,
        const std::string& NegXFilename,
        const std::string& PosYFilename,
        const std::string& NegYFilename,
        const std::string& PosZFilename,
        const std::string& NegZFilename
        */
    );
    void bind(GLenum textureUnit);

    ~CubemapTexture();

    private:
    GLuint textureObj;
};

class Skybox{
    public:
    Skybox();
    void render(glm::mat4 projection, glm::mat4 view, glm::mat4 model);

    private:
    GLuint VBO;
    GLuint VAO;
    int shaderProgram;
    CubemapTexture *cubemap;
    unsigned int texture;

    const char *skyboxVertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "out vec3 TextCoord;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 view;\n"
    "uniform mat4 model;\n"
    "void main()\n"
    "{\n"
    "   vec4 pos = projection * view * vec4(position, 1.0f);\n"
    "   gl_Position = pos.xyww;\n"
    "   TextCoord = position;\n"
    "}\0";
    const char *skyboxFragmentShaderSource = "#version 330 core\n"
    "in vec3 TextCoord;\n"
    "out vec4 FragColor;\n"
    "uniform samplerCube skybox;\n"
    "void main()\n"
    "{\n"
    //"   FragColor = texture(skybox, TextCoord);\n"
    "   FragColor = vec4(1.0, 1.0, TextCoord.z, 0.4);\n"
    "}\n\0";

    GLfloat vertices[108] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

};