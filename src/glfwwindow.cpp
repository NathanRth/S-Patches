#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "render_engine.hpp"
#include "glfwwindow.hpp"
#include "shader.hpp"
#include "debug.hpp"

#include <iostream>
#include <algorithm>

#define SCR_WIDTH 800
#define SCR_HEIGHT 800

GlfwWindow::GlfwWindow() : m_window(nullptr)
{
    m_scene = new Scene();
    m_deltaTime = 0;
    m_lastFrame = 0;
    m_shaderDir = "./shaders/";
}

GlfwWindow::~GlfwWindow()
{

}

int GlfwWindow::init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    m_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "TER 2.0", nullptr, nullptr);

    if (m_window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(m_window);

    //callback functions for input handling
    glfwSetFramebufferSizeCallback(m_window, AbstractEventHandler::framebufferSizeCallback_dispatch);
    glfwSetKeyCallback(m_window, AbstractEventHandler::keyCallback_dispatch);
    glfwSetScrollCallback(m_window, AbstractEventHandler::scrollCallback_dispatch);
    glfwSetMouseButtonCallback(m_window, AbstractEventHandler::mouseButtonCallback_dispatch);
    glfwSetCursorPosCallback(m_window, AbstractEventHandler::cursorPosCallback_dispacth);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    setEventHandling();
    m_engine = new RenderEngine();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(cb_debug, nullptr);
    glEnable(GL_PROGRAM_POINT_SIZE);

    return 0;
}

void GlfwWindow::setupScene()
{
    std::cout << "scene setup" << std::endl;
    // render engine setup
    m_engine->scene(m_scene);

    m_engine->addShader("gbuffer", "./shaders/gbuffer.vs", "./shaders/gbuffer.fs");

    m_engine->addShader("basic", "./shaders/basic.vs", "./shaders/basic.fs");
    m_engine->shader("basic").bind();
    m_engine->shader("basic").setSamplers({"gPosition", "gNormal", "gAlbedo"});
    // scene setup
    m_scene->camera()->setFormat(SCR_HEIGHT, SCR_WIDTH);
    m_scene->addSPatch(SPatch(5,2));
}

void GlfwWindow::show()
{

    Shader flat("./shaders/flat.vs", "./shaders/flat.fs");

    // render loop
    // -----------
    std::cout << "start rendering" << std::endl;

    while (!glfwWindowShouldClose(m_window))
    {

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        flat.bind();
        flat.setUniformMat4("view", m_scene->camera()->viewMatrix());
        flat.setUniformMat4("projection", m_scene->camera()->projectionMatrix());

        for (SPatch spatch : *(m_scene->spatches()))
        {
            flat.setUniformMat4("model",spatch.modelMatrix());
            spatch.draw(flat);
        }

        m_scene->drawAxis(flat);

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void GlfwWindow::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    (void)(window);
    (void)(scancode);
    (void)(mods);

    //exit
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);

    if(key == GLFW_KEY_C && action == GLFW_PRESS)
        m_scene->spatches()->at(0).toggleDrawCP();

    // spatch control
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        m_scene->spatches()->at(0).moveSelectedCP(glm::vec3(0,0,0.1));
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        m_scene->spatches()->at(0).moveSelectedCP(glm::vec3(0,0,-0.1));
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        m_scene->spatches()->at(0).moveSelectedCP(glm::vec3(0,0.1,0));
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        m_scene->spatches()->at(0).moveSelectedCP(glm::vec3(0,-0.1,0));
}

void GlfwWindow::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    (void)(window);
    if (button == GLFW_MOUSE_BUTTON_1)
        glfwGetCursorPos(m_window, &prevXPos, &prevYPos);
    if(button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS)
    {
        int id = m_scene->spatches()->at(0).pickCP(castRay(),m_scene->camera()->position());
        if(id != -1)
        {
            if(mods & GLFW_MOD_SHIFT)
            {
                m_scene->spatches()->at(0).addToSelection(id);
            }
            else
            {
                m_scene->spatches()->at(0).selectedCP(static_cast<std::size_t>(id));
            }
        }
    }
}

void GlfwWindow::scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    (void)(window);
    (void)(xoffset);
    m_scene->camera()->setDist(yoffset);
}

void GlfwWindow::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    (void)(window);
    glViewport(0, 0, width, height);
    std::cout << "width: " << width << " | height: " << height << std::endl;
    m_scene->camera()->setFormat(height, width);
}

void GlfwWindow::cursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
    (void)(window);
    double dx = xpos - prevXPos;
    double dy = ypos - prevYPos;
    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
    {
        // pan view if left shift
        if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            m_scene->camera()->translateBy(dx, dy);
        }
        // rotate view
        else
        {
            m_scene->camera()->rotateBy(dx, dy);
        }
    }
    prevXPos = xpos;
    prevYPos = ypos;
}

glm::vec3 GlfwWindow::castRay()
{
    double xpos;
    double ypos;
    glfwGetCursorPos(m_window, &xpos, &ypos);
    int width = 0;
    int height = 0;
    glfwGetWindowSize(m_window, &width, &height);

    // normalized screen space
    double x = (2.0 * xpos) /  width - 1.0;
    double y = 1.0 - (2.0 * ypos) / height;
    double z = 1.0;
    glm::vec3 ray_nds(x, y, z);

    // clip space
    glm::vec4 ray_clip(ray_nds.x,ray_nds.y, -1.0, 1.0);

    // view space ray
    glm::vec4 ray_eye = inverse(m_scene->camera()->projectionMatrix()) * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

    // world space (normalized)
    glm::vec3 ray_world(inverse(m_scene->camera()->viewMatrix()) * ray_eye);
    ray_world = glm::normalize(ray_world);

    return ray_world;
}
