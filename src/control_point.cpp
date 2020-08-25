#include "control_point.hpp"

ControlPoint::ControlPoint(glm::vec3 pos, glm::vec2 param, Index index) : 
    m_position(pos), m_parametric(param), m_index(index)
{
}

// return NULL if no intersection
// return value of t otherwise
double* ControlPoint::intersect(glm::vec3 O, glm::vec3 D)
{
    static double t;

    glm::vec3 C = m_position;
    glm::vec3 OC = (O-C);
    double r = 0.1;
    double b = static_cast<double>(glm::dot(D,OC));
    double c = static_cast<double>(glm::dot(OC,OC))-r;

    double delta = b*b-c;
    if(delta > 0)
    {
        t = std::min(-b+std::sqrt(b*b - c),-b-std::sqrt(b*b - c));
        return &t;
    }
    if(std::fabs(delta) <= std::numeric_limits<double>::epsilon()) // if delta == 0
    {
        t = -b;
        return &t;
    }
    else
        return nullptr;
}


std::ostream &operator<<(std::ostream &o, ControlPoint &cp)
{
    return o << cp.index() << " (" << cp.m_position.x << ", " << cp.m_position.y << ", " << cp.m_position.z << ")";
}

void ControlPoint::position(glm::vec3 pos)
{
    m_position = pos;
}

void ControlPoint::parametric(glm::vec2 param)
{
    m_parametric = param;
}

glm::vec3 ControlPoint::position() const
{
    return m_position;
}

glm::vec2 ControlPoint::parametric() const
{
    return m_parametric;
}

Index &ControlPoint::index()
{
    return m_index;
}
