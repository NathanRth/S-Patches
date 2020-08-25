#include "object.hpp"
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

Object::Object()
{
    resetTransform();
}

void Object::translate(const glm::vec3 &t)
{
    m_modelMatrix = glm::translate(m_modelMatrix, t);
}

void Object::rotate(float angle, const glm::vec3 &axis)
{
    m_modelMatrix = glm::rotate(m_modelMatrix, angle, axis);
}

void Object::scale(const glm::vec3 &fac)
{
    m_modelMatrix = glm::scale(m_modelMatrix, fac);
}

void Object::scale(const float &fac)
{
    m_modelMatrix = glm::scale(m_modelMatrix, {fac, fac, fac});
}

void Object::resetTransform()
{
    m_modelMatrix = glm::mat4();
}

void Object::position(const glm::vec3 &p)
{
    m_modelMatrix[3][0] = p.x;
    m_modelMatrix[3][1] = p.y;
    m_modelMatrix[3][2] = p.z;
}

glm::vec3 Object::position()
{
    return glm::vec3(m_modelMatrix[3][0], m_modelMatrix[3][1], m_modelMatrix[3][2]);
}
