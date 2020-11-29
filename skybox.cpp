#include "skybox.h"
#include "stb_image.h"


CubemapTexture::CubemapTexture(
    /*
    const std::string& PosXFilename,
    const std::string& NegXFilename,
    const std::string& PosYFilename,
    const std::string& NegYFilename,
    const std::string& PosZFilename,
    const std::string& NegZFilename
    */
){

    int width = 1024, height = 512;
    int comp = 24;
    std::string filenames[6] = {
        //PosXFilename, NegXFilename, PosYFilename, NegYFilename, PosZFilename, NegZFilename
        "image/sky_1.bmp",
        "image/sky_2.bmp",
        "image/sky_3.bmp",
        "image/sky_4.bmp",
        "image/sky.bmp",
        "image/sky.bmp"
        };
    std::cout << filenames[0] << std::endl;
    unsigned char* image;

    glGenTextures(1, &(this->textureObj));
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureObj);
    
    for (int i = 0; i < 6; i++){
        image = stbi_load("image/sky_1.bmp", &width, &height, &comp, STBI_rgb_alpha);
        std::cout << "OK" << std::endl;
        std::cout << sizeof(image) << std::endl;
        std::cout << "OK" << std::endl;
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        stbi_image_free(image);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}

void CubemapTexture::bind(GLenum textureUnit){

    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureObj);
}


Skybox::Skybox(){

    int success;
    char infoLog[512];

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //std::cout << this->axisVertexShaderSource << std::endl;
    glShaderSource(vertexShader, 1, &(this->skyboxVertexShaderSource), NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Error compiling skybox vertex shader\n" << infoLog << std::endl;
    }

    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //std::cout << this->axisFragmentShaderSource << std::endl;

    glShaderSource(fragmentShader, 1, &(this->skyboxFragmentShaderSource), NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Error compiling skybox fragment shader\n" << infoLog << std::endl;
    }

    this->shaderProgram = glCreateProgram();
    glAttachShader(this->shaderProgram, vertexShader);
    glAttachShader(this->shaderProgram, fragmentShader);
    glLinkProgram(this->shaderProgram);

    //std::cout << this->shaderProgram << std::endl;

    glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(this->shaderProgram, 512, NULL, infoLog);
        std::cout << "Error linking skybox shader program\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //this->cubemap = new CubemapTexture();
    

    //std::cout << vertices[0] << std::endl;

    glGenVertexArrays(1, &(this->VAO));
    //std::cout << this->VAO << std::endl;
    glGenBuffers(1, &(this->VBO));
    //std::cout << this->VBO << std::endl;
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned char *image;
    int width = 1024, height = 512;
    int nrChannels = 24;
    image = stbi_load("image/sky.bmp", &width, &height, &nrChannels, 0);
    glGenTextures(1,&(this->texture));
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture);
    for(int i = 0; i<6; i++){
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    }
    
    stbi_image_free(image);

    
    


}

void Skybox::render(glm::mat4 projection, glm::mat4 view, glm::mat4 model){

    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);

    //std::cout << "rendering" << std::endl;
    //std::cout << this->shaderProgram << std::endl;

    //glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    glUseProgram(this->shaderProgram);

    int projLoc = glGetUniformLocation(this->shaderProgram, "projection");
    int viewLoc = glGetUniformLocation(this->shaderProgram, "view");
    int modelLoc = glGetUniformLocation(this->shaderProgram, "model");
    //int skyboxLoc = glGetUniformLocation(this->shaderProgram, "skybox");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    
    glActiveTexture(GL_TEXTURE1);
    std::cout << this->texture << std::endl;
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture);
    glBindVertexArray(this->VAO);
    //this->cubemap->bind(GL_TEXTURE0);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
    

}