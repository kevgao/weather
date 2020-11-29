#include "project.h"
#include "landscape.h"
#include "weather.h"
#include "axis.h"
#include "skybox.h"
#include "particles.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


#include <iostream>
#include <cmath>
//using namespace std;



GLuint WIDTH = 800, HEIGHT = 600;

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
glm::mat4 axisModel;
glm::mat4 landModel;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.5f, -1.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

GLboolean firstMouse = true;
GLfloat cursorX=400, cursorY=300;
GLfloat yaw = -90.0f, pitch = 0.0f;

int loop;
GLfloat slowdown = 1.0;
GLfloat zoom = 0.05f;


Axis *axis;
Landscape *landscape;
Skybox *skybox;
ParticleSystem *snow;


GLuint loadTexture(std::string filename, int width = 1024, int height = 512, int comp = 24){
    
    unsigned char* image = stbi_load(filename.c_str(), &width, &height, &comp, STBI_rgb_alpha);
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    return tex;

}




void init(){

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    model = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
    view = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
    projection = glm::perspective(glm::radians(35.0f), (GLfloat)WIDTH/(GLfloat)HEIGHT, 0.01f, 100.0f);

	axis = new Axis();
    landscape = new Landscape();
    skybox = new Skybox();
    snow = new ParticleSystem();
    axisModel = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    landModel = glm::scale(glm::mat4(1.0f), glm::vec3(50.0f));

	//landscape = new Landscape();

	//generate textures
	/*
    texture[0] = loadTexture("image/sky_1.bmp", 10, 5, idepth);
    texture[1] = loadTexture("image/sky_2.bmp", iwidth, iheight, idepth);
    texture[2] = loadTexture("image/sky_3.bmp", iwidth, iheight, idepth);
    texture[3] = loadTexture("image/sky_4.bmp", iwidth, iheight, idepth);
    texture[4] = loadTexture("image/sky.bmp", iwidth, iheight, idepth);
    texture[5] = loadTexture("Image/shu.bmp");
	texture[6] = loadTexture("Image/shu1.bmp");
	texture[7] = loadTexture("Image/tiao.bmp");
	texture[8] = loadTexture("Image/door.bmp");
	texture[9] = loadTexture("Image/star.bmp");
	texture[10] = loadTexture("Image/shu1_1.bmp");
	texture[11] = loadTexture("Image/shu1_2.bmp");
	texture[12] = loadTexture("Image/shu2_1.bmp");
	texture[13] = loadTexture("Image/shu2_2.bmp");
	texture[14] = loadTexture("Image/cao_1.bmp");
	texture[15] = loadTexture("Image/cao_2.bmp");
	texture[16] = loadTexture("Image/fire.bmp");
	texture[17] = loadTexture("Image/start_1.bmp");	
	texture[18] = loadTexture("Image/start_2.bmp");
	texture[19] = loadTexture("Image/Particle.bmp");
	*/

}

void render(){

    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glLoadIdentity();
	
    skybox->render(projection, view, model);
    
    landscape->render(projection, view, landModel);

    axis->render(projection, view, axisModel);
    
    snow->render();


}


int main( int argc, char *argv[]){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(800, 600, "Weather System", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // glad
    if (! gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 0;
    }

    //int width, height;
    //glfwGetFramebufferSize(window,&width, &height);
    //glViewport(0,0,width, height);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    ImGui::StyleColorsLight();

    init();
    //axis = new Axis(vertices, sizeof(vertices));

    while (!glfwWindowShouldClose(window)){

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        //main render function
        render();
        //axis->render();

        // swap buffer
        glfwSwapBuffers(window);

        // events
        glfwPollEvents();
        

    }

    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0,width,height);
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){

    GLfloat cameraSpeed = 0.05f;
    if(key == GLFW_KEY_W){
        cameraPos += cameraSpeed * cameraFront;
    }
    if(key == GLFW_KEY_S)
        cameraPos -= cameraSpeed * cameraFront;
        //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -0.05));
    if(key == GLFW_KEY_A)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp))*cameraSpeed;
        //view = glm::translate(view, glm::vec3(0.05f, 0.0f, 0.0f));
    if(key == GLFW_KEY_D)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp))*cameraSpeed;
        //view = glm::translate(view, glm::vec3(-0.05f, 0.0f, 0.0f));

    view = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){

    if(firstMouse){
        cursorX = xpos;
        cursorY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - cursorX;
    GLfloat yoffset = cursorY - ypos;
    cursorX = xpos;
    cursorY = ypos;

    GLfloat sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f) pitch = 89.0f;
    if(pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw))*cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw))*cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);

    view = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
    
}
