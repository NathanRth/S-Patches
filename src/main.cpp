#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "glfwwindow.hpp"
#include "debug.hpp"


int main()
{
    GlfwWindow window;
    window.init();
    window.setupScene();
    window.show();
    return 0;
}
