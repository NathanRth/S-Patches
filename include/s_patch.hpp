#ifndef S_PATCH_H
#define S_PATCH_H

#include <glad/glad.h>

#include "object.hpp"
#include "vertex.hpp"
#include "control_point.hpp"

#include <vector>

class SPatch : public Object
{
public:
    SPatch(uint size, uint degree);
    virtual ~SPatch();

    void draw(Shader shader) override;

    const std::vector<ControlPoint>& controlPoints();

    void addToSelection(int i);
    //std::size_t selectedCP() {return m_selectedCP;}
    void selectedCP(std::size_t select);// {m_selectedCP = select;}

    int pickCP(glm::vec3 d, glm::vec3 p);
    void moveSelectedCP(glm::vec3 t);

    void toggleDrawCP();
    
private:
    
    // generate base polygon
    void gen_deg_1();
    
    // generate all distribution possible for a given vector and degree
    void generateDistribution(
        std::vector<Index> *distributions, 
        Index index, 
        uint degree, 
        uint id
    );

    // compute coordinate of cp according to its index
    glm::vec3 computeNewCoord(Index index);

    // compute higher degree
    void computeOnDegree();

    // compute all the connexions in the control polygon
    void computeConnexions();

    // compute the 3D position of a point from its parametric in parametric space
    glm::vec3 evaluateFromMVC(std::vector<double>);
    
    // compute the mvc of a point from its parametric coordinates (should be 2D)
    std::vector<double> computeMVC(glm::vec3 pos, std::vector<ControlPoint>);
    
    // retrieve the upper level of control points in casteljau algorithm
    // TODO move out from class
    std::vector<ControlPoint> getParentCPs(Index, std::vector<ControlPoint>);

    void sample();
    void sampleTriangle(glm::vec2 O,glm::vec2 A,glm::vec2 B);
    void genIndicesList();
    
    uint m_size; // size of the polygon
    uint m_degree; // degre
    uint m_samplingRate; // quality of the mesh surface (higher = prettier)

    // list of control points of the polygon
    std::vector<ControlPoint> m_cp;
    std::vector<ControlPoint> m_base_poly; // only degree = 0

    // list of the mvc coordinates of the sampling points (to be computed once)
    std::vector<std::vector<double>> m_surfaceMVC;

    // ########### Application ###############

    std::vector<std::size_t> m_selectedCPs;
    bool m_drawCP;

    //############### OpenGL #################
    private:
    void make();
    void updateCPBuffer();
    void updateSurfaceBuffer();

    GLuint m_vao_cp, m_vbo_cp, m_ebo_l;
    GLuint m_vao_surf, m_vbo_surf, m_ebo_surf;

    // indices of the connexion edges
    std::vector<uint> m_indices_connex;

    std::vector<Vertex> m_surface;
    std::vector<GLuint> m_surface_indices;
};

#endif
