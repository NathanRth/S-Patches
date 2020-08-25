#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
    Camera(int width, int height);

    void setFormat(int height, int width);
    void setViewAngle(double angle);
    void setNearPlane(double dist);
    void setFarPlane(double dist);

    glm::mat4 viewMatrix() {return m_view;}
    glm::mat4 projectionMatrix() {return m_projection;}

    void computeProjection();

    enum Axis{X,Y,Z};

    void setRotation(double x, double y, double z);
    glm::vec3 rotation() {return glm::vec3(m_rotX, m_rotY, m_rotZ);}
    void setPosition(double x, double y, double z);
    void setPosition(const glm::vec3 &pos);
    glm::vec3 position();

    void rotateBy(double dx, double dy);
    void rotateBy(double angle,Axis axis);
    void translateBy(double dx, double dy);
    void translateBy(const glm::vec3 &v);
    void setDist(double zoom);

    void updateView();

private:


glm::mat4 m_view, m_projection;
    double m_nearPlane, m_farPlane;
    double m_viewAngle;
    int m_width, m_height;

    double m_posX,m_posY,m_posZ;
    double m_rotX,m_rotY,m_rotZ;
    double m_dist;

};

#endif // CAMERA_H
