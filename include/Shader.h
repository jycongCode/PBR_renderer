//
// Created by 从径源 on 2024/6/27.
//

#ifndef HELLOGL_SHADER_H
#define HELLOGL_SHADER_H
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unistd.h>
#include "Light.h"
class Shader{
public:
    unsigned int ID;
    Shader(const char *vertexPath, const char *fragmentPath, const char* geometryPath=NULL) {
        std::string vertexCode;
        std::string fragmentCode;

        std::ifstream vertexFile;
        std::ifstream fragmentFile;

        vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragmentFile.exceptions(std::ifstream ::failbit|std::ifstream ::badbit);

        try{
            vertexFile.open(vertexPath);
            fragmentFile.open(fragmentPath);
            std::stringstream vertexStream,fragmentStream;
            vertexStream << vertexFile.rdbuf();
            fragmentStream << fragmentFile.rdbuf();
            vertexFile.close();
            fragmentFile.close();
            vertexCode = vertexStream.str();
            fragmentCode = fragmentStream.str();
        }catch(std::ifstream::failure e){
            std::cout << "ERROR::SHADER::FILE NOT SUCCESSFULLY READ" << std::endl;
        }

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader,1,&vShaderCode,NULL);
        glCompileShader(vertexShader);
        checkShaderCompileStatus(vertexShader);

        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader,1,&fShaderCode,NULL);
        glCompileShader(fragmentShader);
        checkShaderCompileStatus(fragmentShader);

        this->ID = glCreateProgram();
        glAttachShader(this->ID,vertexShader);
        glAttachShader(this->ID,fragmentShader);

        if(geometryPath != NULL){
            std::string geometryCode;
            std::ifstream geometryFile;
            geometryFile.exceptions(std::ifstream ::failbit|std::ifstream ::badbit);
            try{
                geometryFile.open(geometryPath);
                std::stringstream geometryStream;
                geometryStream << geometryFile.rdbuf();
                geometryFile.close();
                geometryCode = geometryStream.str();
            }catch(std::ifstream::failure e){
                std::cout << "ERROR::SHADER::FILE NOT SUCCESSFULLY READ" << std::endl;
            }
            const char* gShaderCode = geometryCode.c_str();
            geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometryShader,1,&gShaderCode,NULL);
            glCompileShader(geometryShader);
            checkShaderCompileStatus(geometryShader);
            glAttachShader(this->ID,geometryShader);
        }
        glLinkProgram(this->ID);
        checkProgramLinkStatus(this->ID);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
//        glDeleteShader(geometryShader);
    }

    Shader(const char *shaderName,bool hasGeom=false){
        std::string dir(shaderName);
        std::string prefix = "../src/Shaders/" + dir + "/" + dir;
        std::string vertexPath = prefix + ".vert";
        std::string fragmentPath = prefix + ".frag";
        std::string geometryPath = prefix + ".geom";
        if(!hasGeom) geometryPath.clear();
        std::string vertexCode;
        std::string fragmentCode;

        std::ifstream vertexFile;
        std::ifstream fragmentFile;

        vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragmentFile.exceptions(std::ifstream ::failbit|std::ifstream ::badbit);

        try{
            vertexFile.open(vertexPath);
            fragmentFile.open(fragmentPath);
            std::stringstream vertexStream,fragmentStream;
            vertexStream << vertexFile.rdbuf();
            fragmentStream << fragmentFile.rdbuf();
            vertexFile.close();
            fragmentFile.close();
            vertexCode = vertexStream.str();
            fragmentCode = fragmentStream.str();
        }catch(std::ifstream::failure e){
            std::cout << "ERROR::SHADER::FILE NOT SUCCESSFULLY READ" << std::endl;
        }

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader,1,&vShaderCode,NULL);
        glCompileShader(vertexShader);
        checkShaderCompileStatus(vertexShader);

        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader,1,&fShaderCode,NULL);
        glCompileShader(fragmentShader);
        checkShaderCompileStatus(fragmentShader);

        this->ID = glCreateProgram();
        glAttachShader(this->ID,vertexShader);
        glAttachShader(this->ID,fragmentShader);

        if(hasGeom){
            std::string geometryCode;
            std::ifstream geometryFile;
            geometryFile.exceptions(std::ifstream ::failbit|std::ifstream ::badbit);
            try{
                geometryFile.open(geometryPath);
                std::stringstream geometryStream;
                geometryStream << geometryFile.rdbuf();
                geometryFile.close();
                geometryCode = geometryStream.str();
            }catch(std::ifstream::failure e){
                std::cout << "ERROR::SHADER::FILE NOT SUCCESSFULLY READ" << std::endl;
            }
            const char* gShaderCode = geometryCode.c_str();
            geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometryShader,1,&gShaderCode,NULL);
            glCompileShader(geometryShader);
            checkShaderCompileStatus(geometryShader);
            glAttachShader(this->ID,geometryShader);
        }
        glLinkProgram(this->ID);
        checkProgramLinkStatus(this->ID);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        if(hasGeom)glDeleteShader(geometryShader);
    }

    void Destroy(){
        glDeleteProgram(ID);
    }


    void use() {
        glUseProgram(this->ID);
    }

    void setBool(const std::string &name, bool value) const {
        int uniformLocation = glad_glGetUniformLocation(this->ID,name.c_str());
        glUniform1i(uniformLocation,(int)value);
    }

    void setFloat(const std::string &name, float value) const {
        int uniformLocation = glad_glGetUniformLocation(this->ID,name.c_str());
        glUniform1f(uniformLocation,value);
    }

    void setInt(const std::string &name, int value) const {
        int uniformLocation = glad_glGetUniformLocation(this->ID,name.c_str());
        glUniform1i(uniformLocation,value);
    }

    void setMat4(const std::string &name, glm::mat4 value) const {
        int uniformLocation = glad_glGetUniformLocation(this->ID,name.c_str());
        glUniformMatrix4fv(uniformLocation,1,GL_FALSE,glm::value_ptr(value));
    }

    void setVec3(const std::string &name, float x,float y,float z) const {
        int uniformLocation = glad_glGetUniformLocation(this->ID,name.c_str());
        glUniform3f(uniformLocation,x,y,z);
    }

    void setVec3(const std::string &name,glm::vec3 value) const{
        int uniformLocation = glad_glGetUniformLocation(this->ID,name.c_str());
        glUniform3fv(uniformLocation,1,glm::value_ptr(value));
    }

    void setVec2(const std::string &name,glm::vec2 value) const{
        int uniformLocation = glad_glGetUniformLocation(this->ID,name.c_str());
        glUniform2fv(uniformLocation,1,glm::value_ptr(value));
    }

private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int geometryShader;
    void checkProgramLinkStatus(GLuint program){
        int success;
        char infoLog[512];
        glGetProgramiv(program,GL_LINK_STATUS,&success);

        if(!success){
            glGetProgramInfoLog(program,512,NULL,infoLog);
            std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
        }
    }

    void checkShaderCompileStatus(GLuint shader){
        int success;
        char infoLog[512];
        glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
        std::string shaderType;
        if(shader == vertexShader)shaderType = "VERTEX";
        if(shader == fragmentShader)shaderType = "FRAGMENT";
        if(shader == geometryShader)shaderType = "GEOMETRY";
        if(!success){
            glGetShaderInfoLog(shader,512,NULL,infoLog);
            std::cout << "ERROR::SHADER::"<<shaderType<<"::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
};

#endif //HELLOGL_SHADER_H
