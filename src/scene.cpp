#include "scene.hpp"

#include <iostream>

Scene::Scene()
{
    m_camera = new Camera(100, 100);
}

void Scene::addBezierCurve(const BezierCurve &curve)
{
    m_curves.push_back(curve);
}

void Scene::addSPatch(const SPatch &spatch)
{
    m_spatches.push_back(spatch);
}

void Scene::removeBezierCurve(unsigned int index)
{
    m_curves.erase(m_curves.begin() + index);
}

void Scene::drawAxis(Shader shader)
{
    if (m_axis_vao == 0)
    {
        Vertex quadVertices[] = {
            {{0,0,0},{0,0,0},{1,1,1}},
            {{1,0,0},{0,0,0},{1,0,0}},
            {{0,1,0},{0,0,0},{0,1,0}},
            {{0,0,1},{0,0,0},{0,0,1}}
        };
        GLuint indices[] = {0,1,0,2,0,3};

        glGenVertexArrays(1, &m_axis_vao);
        glGenBuffers(1, &m_axis_vbo);
        glGenBuffers(1, &m_axis_ebo);

        glBindVertexArray(m_axis_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_axis_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_axis_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex,normal)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex,color)));
    }
    shader.bind();
    shader.setUniformBool("useVColor", true);
    glBindVertexArray(m_axis_vao);
    glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
