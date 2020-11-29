#include "landscape.h"
#include "stb_image.h"


Landscape::Landscape(){

    int success;
    char infoLog[512];

    // vertex shader
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &(this->landscapeVertexShaderSource), NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "error compiling landscape vertex shader:" << infoLog << std::endl;
    }

    // fragment shader

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &(this->landscapeFragmentShaderSource), NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "error compiling landscape fragment shader:" << infoLog << std::endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "error linking landscape program:" << infoLog << std::endl;
    }

    //glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenVertexArrays(1, &(this->VAO)); 
    glGenBuffers(1, &(this->VBO)); 
    glGenBuffers(1, &(this->EBO));
    
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices), this->indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 8*sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, 8*sizeof(float), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT, GL_FALSE, 8*sizeof(float), (GLvoid*)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned char *image;
    int width = 512, height = 512;
    int nrChannels = 24;
    image = stbi_load("image/di.bmp", &width, &height, &nrChannels, 0);
    std::cout << sizeof(image) << std::endl;
    glGenTextures(1,&(this->texture));
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(image);
}


void Landscape::render(glm::mat4 projection, glm::mat4 view, glm::mat4 model){

    //std::cout << "rendering" << std::endl;
    //std::cout << this->shaderProgram << std::endl;
    glUseProgram(this->shaderProgram);

    int projLoc = glGetUniformLocation(this->shaderProgram, "projection");
    int viewLoc = glGetUniformLocation(this->shaderProgram, "view");
    int modelLoc = glGetUniformLocation(this->shaderProgram, "model");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindTexture(GL_TEXTURE_2D, this->texture);
    //std::cout << "landscape" << this->texture << std::endl;
    glBindVertexArray(this->VAO);
    glLineWidth(8);
    //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // draw the land
    //glDrawArrays(GL_LINE_LOOP,0,6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}
