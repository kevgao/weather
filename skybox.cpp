#include "skybox.h"
#include "stb_image.h"


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

    glGenVertexArrays(1, &(this->VAO));
    glGenBuffers(1, &(this->VBO));

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned char *image;
    int width = 2048, height = 2048;
    int nrChannels = 0;
    glGenTextures(1,&(this->texture));
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture);
    //glBindTexture(GL_TEXTURE_2D, this->texture);
    char* skybox_images[6] = {"image/right.jpg", "image/left.jpg", "image/top.jpg", "image/bottom.jpg", "image/front.jpg", "image/back.jpg"};
    //char* skybox_images[6] = {"image/skybox/right.png", "image/skybox/left.png", "image/skybox/top.png", "image/skybox/bottom.png", "image/skybox/front.png", "image/skybox/back.png"};
    
    for(int i = 0; i<6; i++){
        //stbi_set_flip_vertically_on_load(false);
        image = stbi_load(skybox_images[i], &width, &height, &nrChannels, 0);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    }
    
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    stbi_image_free(image);

    
    


}

void Skybox::render(glm::mat4 projection, glm::mat4 view, glm::mat4 model){

    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);

    //std::cout << "rendering" << std::endl;
    //std::cout << this->shaderProgram << std::endl;

    //glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);
    glUseProgram(this->shaderProgram);

    int projLoc = glGetUniformLocation(this->shaderProgram, "projection");
    int viewLoc = glGetUniformLocation(this->shaderProgram, "view");
    int modelLoc = glGetUniformLocation(this->shaderProgram, "model");
    //int skyboxLoc = glGetUniformLocation(this->shaderProgram, "skybox");
    glm::mat4 skybox_view = glm::mat4(glm::mat3(view));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(skybox_view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    
    //glActiveTexture(GL_TEXTURE1);
    //std::cout << this->texture << std::endl;
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture);
    //glBindTexture(GL_TEXTURE_2D, this->texture);
    glBindVertexArray(this->VAO);
    //this->cubemap->bind(GL_TEXTURE0);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
    //glDepthFunc(GL_LESS);
    

}