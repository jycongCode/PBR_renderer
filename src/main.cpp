#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Window.h"
#include "Skybox.h"
#include "Sphere.h"
#include "Light.h"
#include "Utils.h"
// region time
float deltaTime = 0.0f;
float lastFrame = 0.0f;
void updateTime(){
    float currentTime = (float)glfwGetTime();
    deltaTime = currentTime-lastFrame;
    lastFrame = currentTime;
}
// endregion

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
Window window("PBR",800,800);
Transform transform;
void framebuffer_size_callback(GLFWwindow* window,int width,int height){
    glViewport(0,0,width,height);
}

bool firstMouse = true;
float lastX,lastY;
bool isCamera = false;
bool isObject = false;
float sensitivity = 0.01f;
void mouse_pos_callback(GLFWwindow* window,double xpos,double ypos){
    float xposIn = static_cast<float>(xpos);
    float yposIn = static_cast<float>(ypos);
    if(firstMouse){
        lastX = xposIn,lastY = yposIn;
        firstMouse = false;
    }
    float xoffset = xposIn-lastX;
    float yoffset = lastY-yposIn;
    lastX = xposIn,lastY = yposIn;
    if(isCamera){
        camera.ProcessMouseMovement(xoffset,yoffset, true);
    }
    if(isObject){
        glm::vec3 offset = glm::normalize(camera.Right * xoffset + camera.Up * yoffset);
        glm::vec3 axis = glm::cross(offset,camera.Front);
        float angle = glm::length(glm::vec2(xoffset,yoffset)) * sensitivity;
        transform.Rotate(axis,angle);
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT){
        isCamera = (action == GLFW_PRESS);
    }else if(button == GLFW_MOUSE_BUTTON_LEFT){
        isObject = (action == GLFW_PRESS);
    }
}

void keyboard_process(GLFWwindow* window){
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main()
{
    WindowCallback callbacks(framebuffer_size_callback,mouse_pos_callback,mouse_button_callback);
    window.SetUp(callbacks);
    Skybox *skybox = new Skybox("../img/skybox",false);
    Sphere *sphere = new Sphere();
    Shader shader("sDirLight");
    glm::vec3 lightDir = glm::vec3(-1.0f,-1.0f,0.0f);
    glm::vec3 lightColor = glm::vec3(1.0f,1.0f,1.0f);
    shader.SetDirLight(lightDir,lightColor);
    GLuint sphereTexture = Utils::loadTexture2D("../img/bricks2.jpg", false);
    while(!window.ShouldWindowClose()){
        updateTime();
        keyboard_process(window.GetWindowHandle());
        window.ClearWindow();
        // render
        skybox->Draw(camera,window);
        sphere->Draw(shader,camera,window,transform,sphereTexture);
        window.WindowUpdate();
    }

    skybox->Destroy();
    sphere->Destroy();
    window.CloseWindow();
    delete(skybox);
    delete(sphere);
    return 0;
}



