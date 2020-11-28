#include "landscape.h"

GLfloat Landscape::vertices[] = {
    1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

void Landscape::Landscape(){

    // vertex shader
    const char* vertexShaderSource = VERTEX_SHADER;
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "error compiling vertex shader:" << infoLog << std::endl;
    }

    // fragment shader

    const char* fragmentShaderSource = FRAGMENT_SHADER;
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "error compiling fragment shader:" << infoLog << std::endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "error linking program:" << infoLog << std::endl;
    }

    //glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 8*sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, 8*sizeof(float), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT, GL_FALSE, 8*sizeof(float), (GLvoid*)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
}


void Landscape::render(){

    skybox();
    land();
    house();

    
}

void Landscape::house(){

    glUseProgram(shaderProgram);
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[14]);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


}

void Landscape::land(){

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glPushMatrix();
    glBegin(GL_QUAD_STRIP);
    glColor3f(1,1,1);
    glNormal3d(0,1,0);
    glVertex3f(-0.8, 0, -1);
    glTexCoord2f(0,0);
    glVertex3f(0.8, 0, -1);
    glTexCoord2f(1,0);
    glVertex3f(-0.8, 0, 1);
    glTexCoord2f(0,1);
    glVertex3f(0.8, 0, 1);
    glTexCoord2f(1,1);
    std::cout << "landscape" << std::endl;
    
glEnd();
glPopMatrix();

}




void Landscape::skybox(float x, float y, float z, float box_width, float box_height,float box_length){
    // draw sky
	
	float width =  box_width;
	float height = box_height;
	float length =  box_length;
	
	glPushMatrix();
    glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, texture[0]);
   
	glBegin(GL_QUADS);		
		
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z); 
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[2]);

	glBegin(GL_QUADS);	
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length); 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z + length);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[4]);
	
	glBegin(GL_QUADS);		
		
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length); 
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y + height,	z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height,	z);
		
	glEnd();
	
	
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	
	glBegin(GL_QUADS);		
		
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height,	z);	
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height,	z + length); 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z);		
		
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);		

	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height,	z + length); 
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height,	z);
	glEnd();

    glPopMatrix();                 

}