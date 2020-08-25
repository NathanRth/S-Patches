#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Camera::Camera(int width, int height)
{
    m_width = width;
    m_height = height;

    m_view = glm::mat4();
    m_viewAngle = 45.0;
    m_nearPlane = 0.01;
    m_farPlane = 100.0;

    m_rotX=3.0;
    m_rotY=0;
    m_rotZ=0.0;
    m_posX=0.0;
    m_posY=0.0;
    m_posZ=0.00;
    m_dist=9;
    updateView();
    computeProjection();

}

void Camera::setViewAngle(double angle)
{
    if(angle <= 0.1)
        angle = 0.1;
    else if(angle >180)
        angle = 180;
    else
        m_viewAngle = angle;
    computeProjection();
}

void Camera::setNearPlane(double dist)
{
    if(dist < 0)
        m_nearPlane = 0;
    else if (dist > m_farPlane)
        m_nearPlane = m_farPlane;
    else
        m_nearPlane = dist;
    computeProjection();
}

void Camera::setFarPlane(double dist)
{
    if (dist < m_nearPlane)
        m_farPlane = m_nearPlane;
    else
        m_farPlane = dist;
    computeProjection();
}

void Camera::setFormat(int height, int width)
{
    m_height = height;
    m_width = width;
    computeProjection();
}

void Camera::computeProjection()
{
    m_projection = glm::perspective(
                glm::radians(m_viewAngle),
                static_cast<double>(m_width)/static_cast<double>(m_height),
                m_nearPlane,
                m_farPlane);
}

void Camera::setPosition(double x, double y, double z)
{
    m_posX = x;
    m_posY = y;
    m_posZ = z;
    updateView();
}

void Camera::setPosition(const glm::vec3 &pos)
{
    m_posX = static_cast<double>(pos.x);
    m_posY = static_cast<double>(pos.y);
    m_posZ = static_cast<double>(pos.z);
    updateView();
}

void Camera::setRotation(double x, double y, double z)
{
    m_rotX = x;
    m_rotY = y;
    m_rotZ = z;
    updateView();
}

glm::vec3 Camera::position()
{
    double x = -m_posX + m_dist * -glm::cos(glm::radians(m_rotX)) * glm::sin(glm::radians(m_rotY));
    double y = m_posY + m_dist * glm::sin(glm::radians(m_rotX));
    double z = -m_posZ + m_dist * glm::cos(glm::radians(m_rotX)) * glm::cos(glm::radians(m_rotY));
    return glm::vec3(x,y,z);
}

void Camera::translateBy(double dx, double dy)
{
    dx *= m_dist;
    dy *= m_dist;
    double s = 20;
    m_posX += std::cos(glm::radians(m_rotY))*(dx/s)/30.0;
    m_posZ += std::sin(glm::radians(m_rotY))*(dx/s)/30.0;
    m_posY += std::cos(glm::radians(m_rotX))*(dy/s)/30.0;
    updateView();
}

void Camera::translateBy(const glm::vec3 &v)
{
    m_posX += static_cast<double>(v.x);
    m_posY += static_cast<double>(v.y);
    m_posZ += static_cast<double>(v.z);
    updateView();
}

void Camera::rotateBy(double dx, double dy)
{
    m_rotX+=dy/5.0;
    m_rotY+=dx/5.0;
    updateView();
}

void Camera::rotateBy(double angle, Axis axis)
{
    switch (axis) {
    case X:
        m_rotX += angle;
        break;
    case Y:
        m_rotY += angle;
        break;
    case Z:
        m_rotZ += angle;
        break;
    }
    updateView();
}

void Camera::setDist(double zoom)
{
    m_dist *= 1.0 + (-1.0 * zoom / 30.0);
    updateView();
}

void Camera::updateView()
{
    //std::cout<<"dist: "<<m_dist<<" | pos: "<<m_posX<<" "<<m_posY<<" "<<m_posZ<<" | rot: "<<m_rotX<<" "<<m_rotY<<" "<<m_rotZ<<std::endl;
    m_view = glm::lookAt(glm::vec3(0.0, 0.0, m_dist),glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,1.0,0.0));
    m_view = glm::rotate(m_view,static_cast<float>(glm::radians(m_rotX)), glm::vec3(1.0,0.0,0.0));
    m_view = glm::rotate(m_view,static_cast<float>(glm::radians(m_rotY)), glm::vec3(0.0,1.0,0.0));
    m_view = glm::translate(m_view,glm::vec3(m_posX,-m_posY,m_posZ));
}
