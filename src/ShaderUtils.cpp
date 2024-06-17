#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Global.hpp"
#include "ShaderUtils.hpp"

unsigned int ShaderUtils::getId(){ return(this->ID); }

unsigned int ShaderUtils::GetCompiledShader(unsigned int shader_type, const std::string& shader_source){
    unsigned int shader_id = glCreateShader(shader_type);

    const char* c_source = shader_source.c_str();
    glShaderSource(shader_id, 1, &c_source, nullptr);
    glCompileShader(shader_id);

    GLint result;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);

        GLchar* strInfoLog = new GLchar[length + 1];
        glGetShaderInfoLog(shader_id, length, &length, strInfoLog);

        std::string errMessage = "Compilation error in shader " + static_cast<std::string>(strInfoLog);
        Global::showMessage(errMessage);
        delete[] strInfoLog;

        glDeleteShader(shader_id);
        return 0;
    }

    return shader_id;
}

bool ShaderUtils::Load(const std::string& vertexShaderFile, const std::string& fragmentShaderFile){
    std::ifstream vs_stream(vertexShaderFile);
    std::ifstream fs_stream(fragmentShaderFile);

    if (!vs_stream.is_open() || !fs_stream.is_open()) {
        Global::showMessage("Faile to open shader files");
        return false;
    }

    std::stringstream vs_buffer, fs_buffer;
    vs_buffer << vs_stream.rdbuf();
    fs_buffer << fs_stream.rdbuf();

    unsigned int vs = GetCompiledShader(GL_VERTEX_SHADER, vs_buffer.str());
    unsigned int fs = GetCompiledShader(GL_FRAGMENT_SHADER, fs_buffer.str());

    if (vs == 0 || fs == 0) {
        return false;
    }

    this->ID = glCreateProgram();
    glAttachShader(ID, vs);
    glAttachShader(ID, fs);
    glLinkProgram(ID);
    glValidateProgram(ID);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return true;
}

void ShaderUtils::Use(){
	glUseProgram(ID);
}

void ShaderUtils::Delete(){
	glDeleteProgram(ID);
}

