#ifndef GLFWWINDOW_H
#define GLFWWINDOW_H

#include <string>

#include "abstract_event_handler.hpp"
#include "scene.hpp"

#include <GLFW/glfw3.h>

class RenderEngine;

class GlfwWindow : public AbstractEventHandler
{
  public:
    GlfwWindow();
    virtual ~GlfwWindow();

    int init();
    void setupScene();
    void show();
    GLFWwindow *window() { return m_window; }

    virtual void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    virtual void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    virtual void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    virtual void framebufferSizeCallback(GLFWwindow *window, int width, int height);
    virtual void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);

  private:
    glm::vec3 castRay();
    Scene *m_scene;
    GLFWwindow *m_window;

    int m_frameBufferWidth, m_frameBufferHeight;

    double prevXPos, prevYPos;

    // timing
    float m_deltaTime;
    float m_lastFrame;
    float m_startTime;
    float m_stopTime;

    unsigned int m_frames;

    std::string m_shaderDir;

    RenderEngine* m_engine;
};

#endif // GLFWWINDOW_H
