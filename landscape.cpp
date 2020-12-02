#include "landscape.h"
#include "stb_image.h"


Landscape::Landscape(){

    this->generateIndex();
    this->terrain();
    this->computeNormal();
    /*
    for(int i=0; i<300; i++){
        std::cout << i << ":";
        for(int j=0; j<8; j++){
            std::cout << this->terrainvertices[i*8+j] << ",";
        }
        std::cout << std::endl;
        
    }
    
    for(int i=0; i<(TERRIAN_WIDTH-1)*(TERRIAN_HEIGHT-1); i++){
        std::cout << i << ":";
        for(int j=0; j<6; j++){
            std::cout << this->indices[i*6+j] << ",";
        }
        std::cout << std::endl;
        
    }
    */

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->terrainvertices), this->terrainvertices, GL_STATIC_DRAW);
    
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
    int width = 2048, height = 2048;
    int nrChannels = 0;

    image = stbi_load("image/grassland.jpg", &width, &height, &nrChannels, 0);
    glGenTextures(1,&(this->grass_texture));
    glBindTexture(GL_TEXTURE_2D, this->grass_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(image);

    image = stbi_load("image/snowland.jpg", &width, &height, &nrChannels, 0);
    
    glGenTextures(1,&(this->snow_texture));
    glBindTexture(GL_TEXTURE_2D, this->snow_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(image);
}

void Landscape::terrain(){

    int width = 256;
    int height = 256;
    int nChannels = 0;
    unsigned char *pixelData = stbi_load("image/heightmap1.png", &width, &height, &nChannels, 1);

    float scale_x = ((float)width)/(TERRIAN_WIDTH-1);
    float scale_z = ((float)height)/(TERRIAN_HEIGHT-1);
    
    int image_x;
    int image_y;
    int image_z;
    int position;
    for(int x = 0; x<TERRIAN_WIDTH; x++){
        for(int z = 0; z<TERRIAN_HEIGHT; z++){
            image_x = (int)round(x*scale_x);
            image_y = (int)round(z*scale_z);
            image_z = pixelData[image_x+image_y*width]; //TODO structure of image

            position = 8*(z*TERRIAN_WIDTH+x);
            this->terrainvertices[position] = float(x)/(TERRIAN_WIDTH-1) - 0.5;
            this->terrainvertices[position+1] = (float(image_z)/128.0 - 1.0)/3.0;
            this->terrainvertices[position+2] = float(z)/(TERRIAN_HEIGHT-1) - 0.5;
            this->terrainvertices[position+3] = 0.0;
            this->terrainvertices[position+4] = 0.0;
            this->terrainvertices[position+5] = 0.0;
            this->terrainvertices[position+6] = float(x)/(TERRIAN_WIDTH-1);
            this->terrainvertices[position+7] = float(z)/(TERRIAN_HEIGHT-1);

        }
    }

}

void Landscape::generateIndex(){
    for(int i = 0; i<TERRIAN_WIDTH-1; i++){
        for(int j = 0; j<TERRIAN_HEIGHT-1; j++){
            int position = 6*(i*TERRIAN_WIDTH+j);
            int coord = i*TERRIAN_WIDTH+j;
            this->indices[position] = coord + TERRIAN_HEIGHT + 1;
            this->indices[position+1] = coord+1;
            this->indices[position+2] = coord + TERRIAN_HEIGHT;
            this->indices[position+3] = coord+1;
            this->indices[position+4] = coord;
            this->indices[position+5] = coord+ TERRIAN_HEIGHT;
        }
    }
}

void Landscape::computeNormal(){
    for(int i = 1; i<TERRIAN_WIDTH-1; i++){
        for(int j = 1; j<TERRIAN_HEIGHT-1; j++){
            int position = 8*(i*TERRIAN_WIDTH+j);
            float bottom_vertex = this->terrainvertices[1+8*((i+1)*TERRIAN_WIDTH+j)];
            float up_vertex = this->terrainvertices[1+8*((i-1)*TERRIAN_WIDTH+j)];
            float left_vertex = this->terrainvertices[1+8*(i*TERRIAN_WIDTH+j-1)];
            float right_vertex = this->terrainvertices[1+8*(i*TERRIAN_WIDTH+j+1)];
            glm::vec3 norm = glm::vec3(left_vertex - right_vertex, 2.0f, bottom_vertex - up_vertex);
            norm = glm::normalize(norm);
            
            this->terrainvertices[3+position] = norm.x;
            this->terrainvertices[4+position] = norm.y;
            this->terrainvertices[5+position] = norm.z;

        }
    }
}

void Landscape::render(glm::mat4 projection, glm::mat4 view, glm::mat4 model, int daytime, int weather){

    //std::cout << "rendering" << std::endl;
    //std::cout << this->shaderProgram << std::endl;
    glUseProgram(this->shaderProgram);

    int projLoc = glGetUniformLocation(this->shaderProgram, "projection");
    int viewLoc = glGetUniformLocation(this->shaderProgram, "view");
    int modelLoc = glGetUniformLocation(this->shaderProgram, "model");
    int ambientLoc = glGetUniformLocation(this->shaderProgram, "ambientStrength");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    if(weather == 1){
        glBindTexture(GL_TEXTURE_2D, this->snow_texture);
    }else{
        glBindTexture(GL_TEXTURE_2D, this->snow_texture);
    }

    if(daytime == 0){
        glUniform1f(ambientLoc, 0.8);
    }else{
        glUniform1f(ambientLoc, 0.2);
    }
    
    //std::cout << "landscape" << this->texture << std::endl;
    glBindVertexArray(this->VAO);
    //glLineWidth(8);
    //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // draw the land
    //glDrawArrays(GL_LINE_LOOP,0,6);
    glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(indices[0]), GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLE_STRIP, 0, 65*65);

}
