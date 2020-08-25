#ifndef CONTROL_POINT
#define CONTROL_POINT
#include <iostream>
#include <sstream>
#include <glm/glm.hpp>
#include <vector>

#include "index.hpp"

class ControlPoint
{
public:
    ControlPoint(glm::vec3 pos, glm::vec2 param, Index index);

    double* intersect(glm::vec3 o, glm::vec3 d);

    // setter
    void position(glm::vec3 pos);
    void parametric(glm::vec2 param);
    
    // getter
    glm::vec3 position() const;
    glm::vec2 parametric() const;
    Index& index();

    friend std::ostream& operator<<(std::ostream& o, ControlPoint &cp);

private:
    glm::vec3 m_position;
    glm::vec2 m_parametric;
    Index m_index;
};

#endif
