#ifndef SHADER_H
#define SHADER_H

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
    public:
    // shader program ID
    unsigned int ID;

    Shader(const char *vertexPath, const char *fragmentPath) {
        // 1. Read shader source code from files
        std::string vertexSourceCode;
        std::string fragmentSourceCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        // Enable exceptions on file read failure
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);

            // Read file contents into string streams
            std::stringstream vShaderStream, fShaderStream;

            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            vShaderFile.close();
            fShaderFile.close();

            vertexSourceCode = vShaderStream.str();
            fragmentSourceCode = fShaderStream.str();
        }
        catch(std::ifstream::failure e){
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        // we need to convert our source code to C-strings to compile OpenGL shaders
        const char *vShaderCode = vertexSourceCode.c_str();
        const char *fShaderCode = fragmentSourceCode.c_str();

        // 2. Compilation
        // we store the vertex shader ID and create it using glad
        unsigned int vertex, fragment;
        // Check that the shader source compiles properly
        int success;
        char infoLog[512];

        vertex = glCreateShader(GL_VERTEX_SHADER);
        // Bind the shader source to the OpenGL shader
        // Second arg is how many strings we are passing
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        };

        // Similar for the fragment shaders
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        };

        // Create the shader program, that will be used when rendering objects
        this->ID = glCreateProgram();
        // Attach then link the above compiled shaders to the shaderProgram
        // glAttachShader adds compiled shader stages to a program, while glLinkProgram validates
        // and combines all attached stages into one runnable GPU program.
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use(){
        glUseProgram(this->ID);
    }

    void setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setMat4(const std::string &name, glm::mat4 value) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }
};

#endif
