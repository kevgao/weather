#include "project.h"
#include "landscape.h"
#include "axis.h"
#include "skybox.h"
#include "particles.h"
#include "grass.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include <iostream>
#include <cmath>
//using namespace std;



GLuint WIDTH = 800, HEIGHT = 600;

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
glm::mat4 axisModel;
glm::mat4 landModel;
glm::mat4 particleModel;

glm::vec3 cameraPos = glm::vec3(0.5f, 0.5f, 2.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
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
Grass *grass;

int weather = 1;
int daytime = 0;
bool show_axis = false;
bool show_wind = true;
float wind_strength = 0.0;
glm::vec3 wind = glm::vec3(0.0, 0.0, 0.0);



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
    GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0};
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.8);
    glHint(GL_FOG_HINT, GL_NICEST);
    glFogf(GL_FOG_START, 0.1);
    glFogf(GL_FOG_END, 5);


    model = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
    view = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
    projection = glm::perspective(glm::radians(35.0f), (GLfloat)WIDTH/(GLfloat)HEIGHT, 0.01f, 100.0f);

	axis = new Axis();
    landscape = new Landscape();
    skybox = new Skybox();
    snow = new ParticleSystem();
    grass = new Grass();
    axisModel = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    landModel = glm::scale(glm::mat4(1.0f), glm::vec3(50.0f));
    particleModel = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));

}

void glRender(){

    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glLoadIdentity();
	
    skybox->render(projection, view, model, daytime, weather);
    
    landscape->render(projection, view, landModel, daytime, weather);

    
    snow->render(projection, view, particleModel, daytime, weather, show_wind);
    if(show_axis){
        axis->render(projection, view, axisModel);
    }

    //grass->render(projection, view, model);

    


}

void guiRender(int window_w, int window_h){

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    

    if(ImGui::BeginMainMenuBar()){
        if(ImGui::BeginMenu("File")){
            ImGui::MenuItem("Close", NULL);
            ImGui::EndMenu();

        }
        if(ImGui::BeginMenu("Edit")){
            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("View")){
            ImGui::MenuItem("Reset View", NULL);
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Weather")){
            ImGui::MenuItem("Sunny", NULL);
            ImGui::MenuItem("Snow", NULL);
            ImGui::MenuItem("Rain", NULL);

            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Help")){
            ImGui::MenuItem("About", NULL);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    int boxheight = 260;
    int boxwidth = 80;
    //std::cout << window_h << std::endl;
    ImGui::SetNextWindowSize(ImVec2(boxwidth,boxheight), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(window_w-boxwidth,(window_h-boxheight)/2));
    ImGuiWindowFlags flags = 0
        | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoScrollbar
        | ImGuiWindowFlags_NoResize
        ;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1);
    ImGui::Begin("change vertex", NULL, flags);
    ImGui::PopStyleVar();


    ImGui::Text("Time:");
    ImGui::RadioButton("Day", &daytime, 0);
    ImGui::RadioButton("Night", &daytime, 1);
    //ImGui::RadioButton("Auto", &daytime, 2);

    ImGui::NewLine();

    ImGui::Text("Weather:");
    
    ImGui::RadioButton("Sunny", &weather, 0);
    ImGui::RadioButton("Snow", &weather, 1);
    ImGui::RadioButton("Rain", &weather, 2);
    //ImGui::RadioButton("Hazel", &weather,3);
    /*
    ImGui::Text("Wind:");
    ImGui::DragFloat(NULL, &wind_strength, 0.1f, 0.0f, 1.0f, "%.1f");
    std::cout << wind_strength << std::endl;
    */
    ImGui::NewLine();
    ImGui::Checkbox("Wind", &show_wind);
    ImGui::Checkbox("Axis", &show_axis);

    

    ImGui::End();


    ImGui::Render();

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
    ImGuiIO& io = ImGui::GetIO(); 
    (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    ImGui::StyleColorsLight();

    init();

    int display_w, display_h;
    int window_w, window_h;
    //axis = new Axis(vertices, sizeof(vertices));

    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();

        glfwGetFramebufferSize(window, &display_w, &display_h);
        glfwGetWindowSize(window, &window_w, &window_h);
        
        guiRender(window_w, window_h);
        glRender();
        
        //glViewport(0, 0, display_w, display_h);
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);

        // events
        
        

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
