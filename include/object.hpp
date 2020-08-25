#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include "shader.hpp"

class Object
{
public:
    Object();

    void translate(const glm::vec3 &t);
    void rotate(float angle, const glm::vec3 &axis);
    void scale(const glm::vec3 &fac);
    void scale(const float &fac);
    void resetTransform();
    void position(const glm::vec3 &p);

    glm::vec3 position();

    /**@brief Render the object
     * @param The geometry shader (always the same)
     */
    virtual void draw(Shader shader) = 0;

    glm::mat4 modelMatrix() {return m_modelMatrix;}

protected:

    glm::mat4 m_modelMatrix;
};

#endif // OBJECT_H
