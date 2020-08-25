#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <GLFW/glfw3.h>

class AbstractEventHandler
{
public:

    virtual void setEventHandling() {event_handling_instance = this;}

protected:

    virtual void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) = 0;
    virtual void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) = 0;
    virtual void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) = 0;
    virtual void framebufferSizeCallback(GLFWwindow *window, int width, int height) = 0;
    virtual void cursorPosCallback(GLFWwindow *window, double xpos, double ypos) = 0;

    static AbstractEventHandler * event_handling_instance;


    static void keyCallback_dispatch(GLFWwindow *window,int key,int scancode,int action,int mods)
    {
        if(event_handling_instance)
            event_handling_instance->keyCallback(window,key,scancode,action,mods);
    }
    static void mouseButtonCallback_dispatch(GLFWwindow *window,int button,int action,int mods)
    {
        if(event_handling_instance)
            event_handling_instance->mouseButtonCallback(window,button,action,mods);
    }
    static void scrollCallback_dispatch(GLFWwindow *window,double xoffset, double yoffset)
    {
        if(event_handling_instance)
            event_handling_instance->scrollCallback(window,xoffset, yoffset);
    }
    static void framebufferSizeCallback_dispatch(GLFWwindow *window,int width, int height)
    {
        if(event_handling_instance)
            event_handling_instance->scrollCallback(window,width, height);
    }
    static void cursorPosCallback_dispacth(GLFWwindow *window, double xpos, double ypos)
    {
        if(event_handling_instance)
            event_handling_instance->cursorPosCallback(window,xpos, ypos);
    }
};

#endif // EVENTHANDLER_H
