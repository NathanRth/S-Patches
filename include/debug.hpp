#ifndef DEBUG_H
#define DEBUG_H

#include <GL/gl.h>

void GLAPIENTRY cb_debug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);

#endif