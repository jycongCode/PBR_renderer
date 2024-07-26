#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Window.h"
#include "Skybox.h"
#include "Sphere.h"
#include "Material.h"
#include "Light.h"
#include "IBL.h"
#include "Model.h"
#include <imgui/imgui.h>
#include <imgui/backend/imgui_impl_glfw.h>
#include <imgui/backend/imgui_impl_opengl3.h>
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



float exposure = 0.0f;


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
void mouse_scroll_callback(GLFWwindow* window,double xoffset,double yoffset){
    camera.ProcessMouseScroll(yoffset);
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
    if(glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS) {
        exposure += 0.01;
    }
    if(glfwGetKey(window,GLFW_KEY_DOWN) == GLFW_PRESS) {
        exposure -= 0.01;
    }
}


int main()
{
    WindowCallback callbacks(framebuffer_size_callback,mouse_pos_callback,mouse_button_callback,mouse_scroll_callback);
    window.SetUp(callbacks);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

// Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window.GetWindowHandle(), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();


    PBRMaterial material("../Resources/Textures/RustedIron", true);
    Shader shaderPBR("PBR");
    Shader shaderVis("TangentVis",true);
    DirLight lightDir(-1.0f,0.0f,1.0f,0.5f,0.5f,0.5f);
    IrradianceMap irradianceMap("../Resources/img/skybox",32,32,false);
    SpecularIBL specularMap("../Resources/img/skybox",512,512, false);
    IBL ibl(irradianceMap,specularMap);
    transform.Scale(0.1f,0.1f,0.1f);
    Skybox *skybox = new Skybox("../Resources/img/skybox",false);
    Sphere *sphere = new Sphere();
    Model model("../model/backpack/backpack.obj");
    while(!window.ShouldWindowClose()){
        updateTime();
        keyboard_process(window.GetWindowHandle());
        window.ClearWindow();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        // render
        skybox->Draw(camera,window);
        shaderPBR.use();
        shaderPBR.setFloat("exposure",exposure);
//        sphere->Draw(camera,window,transform,shaderPBR,material,lightDir,ibl);
        model.Draw(camera,window,transform,shaderPBR,material,lightDir,ibl, false);
//        sphere->Draw(camera,window,transform,shaderVis,material,lightDir);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        window.WindowUpdate();
    }
    shaderVis.Destroy();
    shaderPBR.Destroy();
    skybox->Destroy();
    sphere->Destroy();
    model.Destroy();
    irradianceMap.Destroy();
    window.CloseWindow();
    delete(skybox);
    delete(sphere);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    return 0;
}



