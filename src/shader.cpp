//#include <GL/glew.h>
//#include <GL/gl.h>
//#include <GL/glut.h>

#include "shader.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

Shader::Shader(std::string vertexPath, std::string fragmentPath)
{
    m_nbSampler = 0;
    vs = vertexPath;
    fs = fragmentPath;
    // 1. retrieve the vertex/fragment source code from filePath
    std::cout << "Read shaders" << std::endl;
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile(vertexPath);
    std::ifstream fShaderFile(fragmentPath);
    if (vShaderFile.fail())
    {
        std::cerr << "ifstream: cannot open vertex shader file" << std::endl;
    }
    if (fShaderFile.fail())
    {
        std::cerr << "ifstream: cannot opent fragment shader file" << std::endl;
    }
    // ensure ifstream objects can throw exceptions:
    std::cout << "Ensure ifstream completness" << std::endl;
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vertexCode.assign((std::istreambuf_iterator<char>(vShaderFile)),
                          (std::istreambuf_iterator<char>()));
        fragmentCode.assign((std::istreambuf_iterator<char>(fShaderFile)),
                            (std::istreambuf_iterator<char>()));

        //std::cout<<vertexCode<<std::endl;
        //std::cout<<fragmentCode<<std::endl;
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    std::cout << "Compile shaders" << std::endl;
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex Shader
    std::cout << "Compile vertex shaders" << std::endl;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\t" << vs << "\n"
                  << infoLog << std::endl;
    };

    std::cout << "Compile Fragment shader" << std::endl;
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\t" << fs << "\n"
                  << infoLog << std::endl;
    };

    // shader Program
    std::cout << "Create shader program" << std::endl;
    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);
    glLinkProgram(m_id);
    // print linking errors if any
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    // delete the shaders as they're linked into our program now and no longer necessery
    std::cout << "Delete shader" << std::endl;
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::bind()
{
    glUseProgram(m_id);
}

void Shader::setSamplers(std::vector<std::string> samplers)
{
    unsigned int i = 0;
    for (auto s : samplers)
    {
        glUniform1i(glGetUniformLocation(m_id, s.c_str()), (GLint)i++);
    }
}

void Shader::setUniformFloat(std::string name, float val)
{
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), (GLfloat)val);
}

void Shader::setUniformInt(std::string name, int val)
{
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), (GLint)val);
}

void Shader::setUniformUInt(std::string name, unsigned int val)
{
    glUniform1ui(glGetUniformLocation(m_id, name.c_str()), (GLuint)val);
}

void Shader::setUniformBool(std::string name, bool val)
{
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), (GLint)((int)val));
}

void Shader::setUniformVec2(std::string name, glm::vec2 val)
{
    glUniform2f(glGetUniformLocation(m_id, name.c_str()), val.x, val.y);
}

void Shader::setUniformVec3(std::string name, glm::vec3 val)
{
    glUniform3f(glGetUniformLocation(m_id, name.c_str()), val.x, val.y, val.z);
}

void Shader::setUniformMat3(std::string name, glm::mat3 val)
{
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, (GLfloat *)&(val[0]));
}

void Shader::setUniformVec4(std::string name, glm::vec4 val)
{
    glUniform4f(glGetUniformLocation(m_id, name.c_str()), val.x, val.y, val.z, val.w);
}

void Shader::setUniformMat4(std::string name, glm::mat4 val)
{
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, (GLfloat *)&(val[0]));
}

std::ostream &operator<<(std::ostream &o, const Shader &s)
{
    return o << "vertex shader: " << s.vs << " | fragment shader: " << s.fs;
}
