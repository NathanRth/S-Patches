#include "debug.hpp"

#include <string>
#include <iostream>

void GLAPIENTRY cb_debug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    std::string str_source;
    std::string str_severity;
    std::string str_type;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        str_severity = "\033[1;31mHIGH\033[0m";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        str_severity = "\033[1;33mMEDIUM\033[1;37m";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        str_severity = "\033[0;33mLOW\033[0m";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        str_severity = "\033[0;37mNOTIFICATION\033[0m";
        break;
    }

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        str_source = "OpenGL API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        str_source = "Window system";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        str_source = "Application user";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        str_source = "Shader compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        str_source = "Third party application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        str_source = "Other";
        break;
    default:
        str_source = "Undefinied source";
        break;
    }

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        str_type = "API error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        str_type = "Deprecated behavior";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        str_type = "Undefined behavior";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        str_type = "Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        str_type = "Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        str_type = "Command stream annotation";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        str_type = "Push group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        str_type = "Pop group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        str_type = "Other";
        break;
    }

    std::cerr << "[OpenGL ERROR Callback] Source: "<< str_source << " Severity:" << str_severity << " Type:" << str_type << " " << message << "\033[0m"<< std::endl;
}