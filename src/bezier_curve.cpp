#include "bezier_curve.hpp"

#include <glad/glad.h>

#include <iostream>

float randomFloat(float max)
{
    return (float)rand() / (float)(RAND_MAX / max);
}

BezierCurve::BezierCurve()
{
}

BezierCurve::BezierCurve(std::initializer_list<glm::vec3> points)
{
    unsigned int i = 0;
    for (auto e : points)
    {
        m_controlPoints.push_back({e, {0, 1, 0}});
        m_indices.push_back(i++);
    }
    m_focus = 0;
    m_evaluation = 25;
    casteljau();
    makeCurve();
}

void BezierCurve::changeFocus(int i)
{
    if (i < 0)
    {
        if (m_focus > 0)
            m_focus--;
        else
            m_focus = m_controlPoints.size() - 1;
    }
    if (i > 0)
    {
        if (m_focus < m_controlPoints.size() - 1)
            m_focus++;
        else
            m_focus = 0;
    }
}

void BezierCurve::movePoint(glm::vec3 t)
{
    m_controlPoints.at(m_focus).position += t;
    casteljau();
    makeCurve();
}

void BezierCurve::casteljau()
{
    m_curve.clear();
    m_curve_indices.clear();
    glm::vec3 *points;
    points = (glm::vec3 *)malloc((m_controlPoints.size() - 1) * sizeof(glm::vec3));

    for (unsigned int i = 0; i < m_evaluation; i++)
    {
        float t = static_cast<float>(i) / static_cast<float>(m_evaluation - 1);

        for (unsigned int j = 0; j < m_controlPoints.size() - 1; j++)
        {
            points[j] = glm::mix(m_controlPoints[j].position, m_controlPoints[j + 1].position, t);
        }

        for (unsigned int j = m_controlPoints.size() - 2; j > 0; j--)
        {
            for (unsigned int k = 0; k < j; k++)
            {
                points[k] = glm::mix(points[k], points[k + 1], t);
            }
        }
        m_curve.push_back({points[0], {0, 1, 0}});
        m_curve_indices.push_back(i);
    }
    free(points);
}

void BezierCurve::makeCurve()
{
    //std::cout << "make cp" << std::endl;
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_controlPoints.size() * sizeof(Vertex), m_controlPoints.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

    glBindVertexArray(0);

    //std::cout << "make curve" << std::endl;
    glGenVertexArrays(1, &m_vao_2);
    glGenBuffers(1, &m_vbo_2);
    glGenBuffers(1, &m_ebo_2);

    glBindVertexArray(m_vao_2);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_2);
    glBufferData(GL_ARRAY_BUFFER, m_curve.size() * sizeof(Vertex), m_curve.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo_2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_curve_indices.size() * sizeof(unsigned int), m_curve_indices.data(), GL_STATIC_DRAW);

    // vertex positions
    //glBindVertexArray(m_vao_2);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

    glBindVertexArray(0);
    //std::cout << "done" << std::endl;
}

void BezierCurve::draw(Shader shader)
{
    shader.bind();

    // control points
    shader.setUniformVec3("color", {0.5f, 0.5f, 0.5f});
    glBindVertexArray(m_vao);
    glDrawElements(GL_LINE_STRIP, m_indices.size(), GL_UNSIGNED_INT, 0);
    shader.setUniformVec3("color", {0.5f, 0.5f, 0.75f});
    glDrawElements(GL_POINTS, m_indices.size(), GL_UNSIGNED_INT, 0);

    // curve
    shader.setUniformVec3("color", {1.0f, 1.0f, 1.0f});
    glBindVertexArray(m_vao_2);
    glDrawElements(GL_LINE_STRIP, m_curve_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}