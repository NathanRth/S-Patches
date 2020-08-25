#ifndef BEZIER_CURVE_H
#define BEZIER_CURVE_H

#include "object.hpp"

#include "vertex.hpp"

#include <initializer_list>
#include <vector>

class BezierCurve : public Object
{
  public:
    BezierCurve();

    // expects a brace-list of glm::vec3 elements
    BezierCurve(std::initializer_list<glm::vec3> points);

    // set up buffer for opengl rendering
    void makeCurve();

    // generate bezier curve by sampling control points using casteljau algorithm
    void casteljau();
    
    // perform degree elevation to ad control points
    void elevate();

    // move the selected control point then perform a curve update
    void movePoint(glm::vec3 t);

    // change the index of the focused control point
    void changeFocus(int i);

    // draw the control points and the curve
    void draw(Shader shader);

  private:
    unsigned int m_focus;
    unsigned int m_evaluation;
    std::vector<Vertex> m_curve;
    std::vector<unsigned int> m_curve_indices;
    std::vector<Vertex> m_controlPoints;
    std::vector<unsigned int> m_indices;

    unsigned int m_vao, m_vbo, m_ebo;
    unsigned int m_vao_2, m_vbo_2, m_ebo_2;
};

#endif