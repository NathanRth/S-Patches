#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <glad/glad.h>

#include <map>
#include <string>

#include "shader.hpp"
#include "scene.hpp"

class RenderEngine
{
    public:
    RenderEngine();

    // setter
    void scene(Scene *scene);
    
    void render();
    void renderBuffer();

    void geometryPass(std::string shaderName);
    void lightingPass(std::string shaderName);

    Shader &shader(std::string name);

    void addShader(std::string name, std::string vs, std::string fs);

    private:
    GLuint gPosition, gAlbedo, gNormal, gDepth, gBuffer;
    GLuint rboDepth;
    GLuint quadVAO, quadVBO;

    Scene *m_scene;

    std::map<std::string, Shader> m_shaderCollection;
};

#endif