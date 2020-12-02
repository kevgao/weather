#define GLFW_INCLUDE_NONE
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

#define MAX_PARTICLE_COUNT 10000

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
    
    void update(int daytime, int weather, bool show_wind);
    
    void render(glm::mat4 projection, glm::mat4 view, glm::mat4 model, int daytime, int weather, bool show_wind);

    private:
    GLint particleCount;
    int shaderProgram;
    GLuint VAO;
    GLuint vertexVBO;
    GLuint particleVBO;
    GLuint snow_texture;
    GLuint rain_texture;

    const char *particleVertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 position;\n"
    "layout (location = 2) in vec3 velocity;\n"
    "layout (location = 3) in vec4 color;\n"
    "layout (location = 4) in float age;\n"
    "layout (location = 5) in float life;\n"
    "layout (location = 6) in float size;\n"
    "out vec4 Color;\n"
    "out vec2 textCoord;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 view;\n"
    "uniform mat4 model;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * vec4(size * aPos + position, 1.0f);\n"
    "   Color = color;\n"
    "   textCoord = vec2(aPos.x, aPos.y);\n"
    "}\0";
    const char *particleFragmentShaderSource = "#version 330 core\n"
    "in vec4 Color;\n"
    "in vec2 textCoord;\n"
    "out vec4 fragColor;\n"
    "uniform sampler2D tex;\n"
    "void main()\n"
    "{\n"
    //"   fragColor = vec4(Color);\n"
    //"   fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
    "   vec4 texColor = texture(tex, textCoord);\n"
    "   if(texColor.r > 0.9)discard;\n"
    //"   fragColor = vec4(1.0f, 1.0f, 1.0f, texColor.a);\n"
    "   fragColor = texColor;\n"
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
    Particle particles[MAX_PARTICLE_COUNT];

    Particle generateParticle();
    

};