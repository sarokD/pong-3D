#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "material.hpp"

struct Shader {
    unsigned int ID;

    Shader(const std::string& vsPath, const std::string& fsPath) {
        std::ifstream vsFile;
        std::ifstream fsFile;
        std::stringstream vsStream, fsStream;

        vsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            vsFile.open(vsPath);
            fsFile.open(fsPath);

            vsStream << vsFile.rdbuf();
            fsStream << fsFile.rdbuf();

        } catch (std::ifstream::failure& e) {
            std::cerr << "Failed reading file\n";
        }
        std::string vsSrc = vsStream.str();
        std::string fsSrc = fsStream.str();
        const char *vsCode = vsSrc.c_str();
        const char *fsCode = fsSrc.c_str();

        unsigned int vs, fs;
        int success;
        char infoLog[512];

        vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vsCode, NULL);
        glCompileShader(vs);
        glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vs, sizeof infoLog, NULL, infoLog);
            std::cerr << "Failed to compile vertex shader\n" << infoLog << '\n';
        }
        fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fsCode, NULL);
        glCompileShader(fs);
        glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fs, sizeof infoLog, NULL, infoLog);
            std::cerr << "Failed to compile fragment shader\n" << infoLog << '\n';
        }

        ID = glCreateProgram();
        glAttachShader(ID, vs);
        glAttachShader(ID, fs);
        glLinkProgram(ID);
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, sizeof infoLog, NULL, infoLog);
            std::cerr << "Failed to link program\n" << infoLog << '\n';
        }

        glDeleteShader(vs);
        glDeleteShader(fs);
    }

    Shader(const Shader& other) = delete;
    Shader& operator=(const Shader& other) = delete;

    ~Shader() {
        glDeleteProgram(ID);
    }

    inline void use() const{
        glUseProgram(ID);
    }

    void set(const std::string& name, float value) const
    { glUniform1f(glGetUniformLocation(ID, name.c_str()), value); }

    void set(const std::string& name, int value) const
    { glUniform1i(glGetUniformLocation(ID, name.c_str()), value); }

    void set(const std::string& name, const glm::vec3& value) const
    { glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); }

    void set(const std::string& name, const glm::mat4& value) const
    { glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]); }

    void set(const std::string& name, const Material value) const {
        set(name+".ambient", value.ambient);
        set(name+".diffuse", value.diffuse);
        set(name+".specular", value.specular);
        set(name+".shininess", value.shininess);
    }

};