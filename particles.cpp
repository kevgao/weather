#include "particles.h"
#include "stb_image.h"
#include <random>

ParticleSystem::ParticleSystem(){
    
    this->particleCount = MAX_PARTICLE_COUNT;

    for(int i = 0; i<this->particleCount; i++){
        this->particles[i] = this->generateParticle();
        //std::cout << this->particles[i].position.x << std::endl;
        
    };

    int success;
    char infoLog[512];

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //std::cout << this->axisVertexShaderSource << std::endl;
    glShaderSource(vertexShader, 1, &(this->particleVertexShaderSource), NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Error compiling particle vertex shader\n" << infoLog << std::endl;
    }

    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //std::cout << this->axisFragmentShaderSource << std::endl;

    glShaderSource(fragmentShader, 1, &(this->particleFragmentShaderSource), NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Error compiling particle fragment shader\n" << infoLog << std::endl;
    }

    this->shaderProgram = glCreateProgram();
    glAttachShader(this->shaderProgram, vertexShader);
    glAttachShader(this->shaderProgram, fragmentShader);
    glLinkProgram(this->shaderProgram);

    //std::cout << this->shaderProgram << std::endl;

    glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(this->shaderProgram, 512, NULL, infoLog);
        std::cout << "Error linking particle shader program\n" << infoLog << std::endl;
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
    
    glGenBuffers(1, &(this->particleVBO));
    glBindBuffer(GL_ARRAY_BUFFER, this->particleVBO);
    glBufferData(GL_ARRAY_BUFFER, this->particleCount*sizeof(Particle), this->particles, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(10*sizeof(float)));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(11*sizeof(float)));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(12*sizeof(float)));

    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    

    glBindVertexArray(0);

    unsigned char *image;
    int width = 1348, height = 1172;
    int nrChannels = 0;
    image = stbi_load("image/xuehua.jpg", &width, &height, &nrChannels, 0);
    glGenTextures(1,&(this->snow_texture));
    glBindTexture(GL_TEXTURE_2D, this->snow_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(image);

    width = 360;
    height = 386;
    nrChannels = 0;
    stbi_set_flip_vertically_on_load(true);
    image = stbi_load("image/raindrop2.png", &width, &height, &nrChannels, 0);
    glGenTextures(1,&(this->rain_texture));
    glBindTexture(GL_TEXTURE_2D, this->rain_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(image);

}

Particle ParticleSystem::generateParticle(){
    Particle part;
    static std::default_random_engine engine{};
    static std::uniform_real_distribution<float> distribution{0.0, 1.0};
    static std::uniform_real_distribution<float> rdistribution{-1.0, 1.0};
    part.position = glm::vec3(5*rdistribution(engine), 3*distribution(engine), 10*rdistribution(engine));
    part.color = glm::vec4(distribution(engine), distribution(engine), distribution(engine), 1.0);
    part.velocity = glm::vec3(0, -0.002f, 0.0);
    part.life = 10.0;
    part.age = 1.0;
    part.size = 0.01*(1.0+distribution(engine));

    return part;

}

void ParticleSystem::render(glm::mat4 projection, glm::mat4 view, glm::mat4 model, int daytime, int weather){

    glUseProgram(this->shaderProgram);

    int projLoc = glGetUniformLocation(this->shaderProgram, "projection");
    int viewLoc = glGetUniformLocation(this->shaderProgram, "view");
    int modelLoc = glGetUniformLocation(this->shaderProgram, "model");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    if(weather == 1){
        glBindTexture(GL_TEXTURE_2D, this->snow_texture);
        this->update(daytime, weather);
        glBindVertexArray(this->VAO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, this->particleCount);
    }else if(weather == 2){
        glBindTexture(GL_TEXTURE_2D, this->rain_texture);
        this->update(daytime, weather);
        glBindVertexArray(this->VAO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, this->particleCount);
    }
    
    
    //std::cout << "landscape" << this->texture << std::endl;
    
    //glDrawArrays(GL_TRIANGLES, 0, 6);

}

void ParticleSystem::update(int daytime, int weather){

    for(int i = 0; i<this->particleCount; i++){
        if(weather == 1){
            this->particles[i].position+= this->particles[i].velocity;
        }else if(weather == 2){
            this->particles[i].position+= glm::vec3(1.0, 10.0, 1.0)*this->particles[i].velocity;
        }
        
        if(this->particles[i].position.y<0.1){
            this->particles[i].position.y=3.0;
        }
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, this->particleVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, this->particleCount*sizeof(Particle), this->particles);

    //std::cout << "updated." << std::endl;
}