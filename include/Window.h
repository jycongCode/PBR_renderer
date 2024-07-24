//
// Created by 从径源 on 2024/7/22.
//

#ifndef LEARNOPENGL_WINDOW_H
#define LEARNOPENGL_WINDOW_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

struct WindowCallback{
    WindowCallback(GLFWframebuffersizefun fsc,GLFWcursorposfun mpc,GLFWmousebuttonfun mbc,GLFWscrollfun msc){
        framebuffer_size_callback = fsc;
        mouse_pos_callback = mpc;
        mouse_button_callback = mbc;
        mouse_scroll_callback = msc;
    }

    GLFWframebuffersizefun framebuffer_size_callback;
    GLFWcursorposfun mouse_pos_callback;
    GLFWmousebuttonfun mouse_button_callback;
    GLFWscrollfun  mouse_scroll_callback;
};

class Window{
public:
    Window(const char* name,int width,int height):scrWidth(width),scrHeight(height){
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        windowHandle = glfwCreateWindow(width, height, name, NULL, NULL);
        if (windowHandle == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }
    }
    void SetUp(WindowCallback callbacks){
        glfwMakeContextCurrent(windowHandle);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }

        glViewport(0, 0, scrWidth, scrHeight);
        glfwSetFramebufferSizeCallback(windowHandle, callbacks.framebuffer_size_callback);
        glfwSetCursorPosCallback(windowHandle,callbacks.mouse_pos_callback);
        glfwSetMouseButtonCallback(windowHandle, callbacks.mouse_button_callback);
        glfwSetScrollCallback(windowHandle,callbacks.mouse_scroll_callback);
    }
    bool ShouldWindowClose(){
        return glfwWindowShouldClose(windowHandle);
    }

    void WindowUpdate(){
        glfwSwapBuffers(windowHandle);
        glfwPollEvents();
    }

    void ClearWindow(){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    void CloseWindow(){glfwTerminate();}
    int GetWindowWidth() {return scrWidth;}
    int GetWindowHeight() {return scrHeight;}
    GLFWwindow* GetWindowHandle(){return windowHandle;}
private:
    GLFWwindow *windowHandle;
    int scrWidth;
    int scrHeight;
};
#endif //LEARNOPENGL_WINDOW_H
