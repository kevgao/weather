#define GLFW_INCLUDE_NONE
#include "glad.h"
#include <GLFW/glfw3.h>


#define VERTEX_SHADER ("#version 330 core\n\
layout (location = 0) in vec3 position;\n\
layout (location = 1) in vec3 dnorm;\n\
layout (location = 2) in vec2 dtexCoord;\n\
out vec3 norm;\n\
out vec2 texCoord;\n\
uniform mat4 model;\n\
uniform mat4 view;\n\
uniform mat4 projection;\n\
void main(){\n\
gl_Position =  projection * view * model * vec4(position, 1.0f);\n\
norm = dnorm; \n\
texCoord = dtexCoord; \n\
}\0")

#define FRAGMENT_SHADER ("#version 330 core\n\
in vec3 norm;\n\
in vec2 texCoord;\n\
out vec4 fragColor;\n\
uniform sampler2D ourTexture;\n\
void main(){\n\
fragColor = texture(ourTexture, texCoord);\n\
}\0")


class Landscape{
    public:
    void render();

    private:

    GLuint VAO;
    GLuint VBO;
    GLuint shaderProgram;
    void house();
    void skybox(float x, float y, float z, float box_width, float box_height,float box_length);
    void land();

    GLfloat vertices[];

};
