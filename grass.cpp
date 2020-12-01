#include "grass.h"
#include "stb_image.h"
#include <random>

Grass::Grass(){
    
    this->bladeCount = MAX_BLADE_COUNT;

    

    for(int i = 0; i<this->bladeCount; i++){
        this->blades[i] = this->generateBlade();
        
    };
    
    int success;
    char infoLog[512];

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //std::cout << this->axisVertexShaderSource << std::endl;
    glShaderSource(vertexShader, 1, &(this->bladeVertexShaderSource), NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Error compiling blade vertex shader\n" << infoLog << std::endl;
    }

    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //std::cout << this->axisFragmentShaderSource << std::endl;

    glShaderSource(fragmentShader, 1, &(this->bladeFragmentShaderSource), NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Error compiling blade fragment shader\n" << infoLog << std::endl;
    }

    this->shaderProgram = glCreateProgram();
    glAttachShader(this->shaderProgram, vertexShader);
    glAttachShader(this->shaderProgram, fragmentShader);
    glLinkProgram(this->shaderProgram);

    //std::cout << this->shaderProgram << std::endl;

    glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(this->shaderProgram, 512, NULL, infoLog);
        std::cout << "Error linking blade shader program\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    glGenVertexArrays(1, &(this->VAO));
    glBindVertexArray(this->VAO);

    glGenBuffers(1, &(this->vertexVBO));
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    
    glGenBuffers(1, &(this->bladeVBO));
    glBindBuffer(GL_ARRAY_BUFFER, this->bladeVBO);
    glBufferData(GL_ARRAY_BUFFER, this->bladeCount*sizeof(Blade), this->blades, GL_DYNAMIC_DRAW);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Blade), (void*)0);
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Blade), (void*)(3*sizeof(float)));
    
    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    
    

    

    glBindVertexArray(0);

    unsigned char *image;
    int width = 1024, height = 1024;
    int nrChannels = 0;
    stbi_set_flip_vertically_on_load(true);
    image = stbi_load("image/sky.bmp", &width, &height, &nrChannels, 0);
    glGenTextures(1,&(this->texture));
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(image);

}

Blade Grass::generateBlade(){
    Blade part;
    static std::default_random_engine engine{};
    static std::uniform_real_distribution<float> distribution{0.0, 1.0};
    static std::normal_distribution<float> rdistribution{0.0, 1.0};
    part.position = glm::vec3(5*rdistribution(engine), 0,10*rdistribution(engine));
    part.distortion = 10.0;

    return part;

}

void Grass::render(glm::mat4 projection, glm::mat4 view, glm::mat4 model){

    glUseProgram(this->shaderProgram);

    int projLoc = glGetUniformLocation(this->shaderProgram, "projection");
    int viewLoc = glGetUniformLocation(this->shaderProgram, "view");
    int modelLoc = glGetUniformLocation(this->shaderProgram, "model");
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindTexture(GL_TEXTURE_2D, this->texture);

    

    this->update();
    
    //std::cout << "landscape" << this->texture << std::endl;
    glBindVertexArray(this->VAO);
    
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, this->bladeCount);
    
    //glDrawArrays(GL_TRIANGLES, 0, 6);

}

void Grass::update(){
    glBindBuffer(GL_ARRAY_BUFFER, this->bladeVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, this->bladeCount*sizeof(Blade), this->blades);
}