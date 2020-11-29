#include "particles.h"
#include <random>

ParticleSystem::ParticleSystem(){
    
    this->particleCount = 100;

    

    for(int i = 0; i<100; i++){
        this->particles[i] = this->generateParticle();
        
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
    glBufferData(GL_ARRAY_BUFFER, this->particleCount * sizeof(Particle), this->particles, GL_STREAM_DRAW);
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

}

Particle ParticleSystem::generateParticle(){
    Particle part;
    static std::default_random_engine engine{};
    static std::uniform_real_distribution<float> distribution{0.0, 1.0};
    part.position = glm::vec3(distribution(engine), distribution(engine), 0.0);
    part.color = glm::vec4(distribution(engine), distribution(engine), distribution(engine), 1.0);
    part.velocity = glm::vec3(distribution(engine), distribution(engine), 0.0);
    part.life = 10.0;
    part.age = distribution(engine);
    part.size = 1.0;

    return part;

}

void ParticleSystem::render(){
    
    glUseProgram(this->shaderProgram);
    glBindVertexArray(this->VAO);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, this->particleCount);

}