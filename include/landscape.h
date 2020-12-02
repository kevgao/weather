#define GLFW_INCLUDE_NONE
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

#define TERRIAN_WIDTH 129
#define TERRIAN_HEIGHT 129

class Landscape{
    public:
    Landscape();
    void render(glm::mat4 projection, glm::mat4 view, glm::mat4 model, int daytime, int weather);
    void terrain();
    void generateIndex();
    void computeNormal();

    private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint shaderProgram;
    unsigned int grass_texture;
    unsigned int snow_texture;
    

    const char* landscapeVertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "layout (location = 1) in vec3 dnorm;\n"
        "layout (location = 2) in vec2 dtexCoord;\n"
        "out vec3 norm;\n"
        "out vec2 texCoord;\n"
        "out vec3 fragPos;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "void main(){\n"
        "   gl_Position =  projection * view * model * vec4(position, 1.0f);\n"
        "   fragPos = vec3(model * vec4(position, 1.0f));\n"
        "   norm = dnorm;\n"
        "   texCoord = dtexCoord;\n"
        "}\0";

    const char* landscapeFragmentShaderSource = "#version 330 core\n"
        "in vec3 norm;\n"
        "in vec2 texCoord;\n"
        "in vec3 fragPos;\n"
        "out vec4 fragColor;\n"
        "uniform sampler2D ourTexture;\n"
        "uniform float ambientStrength;\n"
        
        "void main(){\n"
        "   vec3 light_pos = vec3(5.0f, 3.0f, 3.0f);\n"
        "   vec3 light_color = vec3(1.0f, 1.0f, 1.0f);\n"
        "   vec3 light_dir = normalize(light_pos - fragPos);\n"
        "   float diff =max(dot(norm, light_dir),0.0);\n"
        "   vec3 diffuse = diff*light_color;\n"
        "   vec3 ambient = ambientStrength*light_color;\n"
        "   vec4 texColor = texture(ourTexture, texCoord);\n"
        "   vec3 result = (ambient+diffuse)*vec3(texColor);\n"
        "   fragColor = vec4(result, 1.0);\n"
        
        //"   fragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
        "}\0";
    /*
    GLfloat terrainvertices[32] = {
        0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };
    GLuint indices[6] = {
        0, 2, 1,
        1, 2, 3
    };
    */
    /*
    GLfloat terrainvertices[32] = {
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };
    GLuint indices[6] = {
        0,2,1,
        2,3,1
    };
    */
    
    GLuint indices[6*(TERRIAN_WIDTH-1)*(TERRIAN_HEIGHT-1)];


    GLfloat terrainvertices[TERRIAN_WIDTH*TERRIAN_HEIGHT*8];
    


};
