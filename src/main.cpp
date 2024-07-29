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
Window window("PBR",1280,700);
Transform transform;
void framebuffer_size_callback(GLFWwindow* window,int width,int height){
    glViewport(0,0,width,height);
}

bool firstMouse = true;
float lastX,lastY;
bool isCamera = false;
bool isObject = false;
float sensitivity = 0.01f;
bool isMouseFocused = true;

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
    if(isMouseFocused){
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

bool isHDR = false;
float exposure = 1.0f;

bool gammaCorrection = false;
float gammaParam = 2.2f;
void keyboard_process(GLFWwindow* window){
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

bool showSkyBox = true;
bool isPC = false;

ImVec4 albedoColor = ImVec4(0.5f,0.5f,0.5f,1.0f);
float metallic = 0.5f;
float roughness = 0.5f;

ImVec4 lightColor = ImVec4(0.5f,0.5f,0.5f,1.0f);
float lightIntensity = 1.0f;
bool isLightOn = false;

bool isCubeMapOn = true;
float scale = 0.5f;
int main()
{
    WindowCallback callbacks(framebuffer_size_callback,mouse_pos_callback,mouse_button_callback,mouse_scroll_callback);
    window.SetUp(callbacks);

    // imgui setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    ImGui::StyleColorsDark();
// Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window.GetWindowHandle(), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsLight();


    PBRMaterial material("../Resources/Textures/RustedIron", true);
    PBRpcMaterial materialpc(0.5f,0.5f,"../Resources/Textures/RustedIron/normal.png");
    Shader shaderPBR("PBR");
    Shader shaderPBRpc("PBRpc");
    Shader shaderVis("TangentVis",true);
//    DirLight lightDir(-1.0f,0.0f,1.0f,0.5f,0.5f,0.5f,1.0f);
//    PointLight lightPoint(10.0f,10.0f,10.0f,0.5f,0.5f,0.5f,100.0f);
    Light light;
    light.Position = glm::vec3(10.0f,10.0f,10.0f);
    IrradianceMap irradianceMap("../Resources/img/skybox",32,32,false);
    SpecularIBL specularMap("../Resources/img/skybox",512,512, false);
    IBL ibl(irradianceMap,specularMap);
    Skybox *skybox = new Skybox("../Resources/img/skybox",false);
    Sphere *sphere = new Sphere();
    Model model("../model/nanosuit/nanosuit.obj");

    while(!window.ShouldWindowClose()){
        updateTime();
        keyboard_process(window.GetWindowHandle());
        window.ClearWindow();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        isMouseFocused = !io.WantCaptureMouse;
        ImGui::Begin("Configuration");
        if(ImGui::CollapsingHeader("HDR & Gamma Correction")){
            ImGui::Checkbox("Use HDR",&isHDR);
            if(isHDR){
                ImGui::SliderFloat("exposure",&exposure,0.1f,2.0f);
            }
            ImGui::Checkbox("Gamma Correction",&gammaCorrection);
            if(gammaCorrection){
                ImGui::SliderFloat("gamma",&gammaParam,2.1f,2.3f);
            }
        }

        ImGui::Checkbox("Pure Color",&isPC);
        if(isPC){
            if(ImGui::CollapsingHeader("PBR parameters")){
                ImGui::ColorEdit3("albedo",(float*)&albedoColor);
                ImGui::SliderFloat("metallic",&metallic,0.0f,1.0f);
                ImGui::SliderFloat("roughness",&roughness,0.0f,1.0f);
            }
            materialpc.roughness = roughness;
            materialpc.metallic = metallic;
            materialpc.albedo = glm::vec3(albedoColor.x*albedoColor.w,albedoColor.y*albedoColor.w,albedoColor.z*albedoColor.w);
        }

        ImGui::Checkbox("Direct Light",&isLightOn);
        if(isLightOn){
            if(ImGui::CollapsingHeader("Direct Light Parameter")){
                ImGui::ColorEdit3("light Color",(float*)&lightColor);
                ImGui::DragFloat("Intensity",&lightIntensity);
            }
            light.Color = glm::vec3(lightColor.x*lightColor.w,lightColor.y*lightColor.w,lightColor.z*lightColor.w);
            light.Intensity = lightIntensity;
        }

        if(ImGui::CollapsingHeader("Transform")){
            ImGui::DragFloat("Scale",&scale,0.01f);
            transform.Scale(scale,scale,scale);
        }

        if(ImGui::CollapsingHeader("Camera")){
            ImGui::DragFloat("ArmHeight",&camera.ArmHeight,0.01f);
            camera.updateCameraVectors();
        }
        ImGui::Checkbox("Indirect Light",&isCubeMapOn);
        ImGui::Checkbox("Show Skybox",&showSkyBox);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::ShowDemoWindow();
        ImGui::End();

        ImGui::Render();
        Shader shaderToDraw;
        if(isPC){
            shaderToDraw = shaderPBRpc;
        }else{
            shaderToDraw = shaderPBR;
        }
        // render
        if(showSkyBox){
            skybox->Draw(camera,window);
        }
        shaderToDraw.use();
        shaderToDraw.setBool("hdr",isHDR);
        shaderToDraw.setFloat("exposure",exposure);
        shaderToDraw.setBool("gammaCorrection",gammaCorrection);
        shaderToDraw.setFloat("gamma",gammaParam);
        shaderToDraw.setBool("isDirectLight",isLightOn);
        shaderToDraw.setBool("isEnvironmentLight",isCubeMapOn);
        if(isPC){
//            sphere->Draw(camera,window,transform,shaderToDraw,materialpc,light,ibl);
            model.Draw(camera,window,transform,shaderToDraw,materialpc,light,ibl, false);
        }else{
//            sphere->Draw(camera,window,transform,shaderToDraw,material,light,ibl);
            model.Draw(camera,window,transform,shaderToDraw,material,light,ibl, false);
        }
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



