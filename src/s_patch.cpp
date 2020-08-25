#define GLM_ENABLE_EXPERIMENTAL

#include "s_patch.hpp"
#include "vertex.hpp"
#include <cmath>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/string_cast.hpp>
#include <time.h>

#include "utils.hpp"


SPatch::SPatch(uint size, uint degree)
{
    m_size = size;
    m_degree = degree;
    m_samplingRate = 8;
    m_drawCP = true;

    gen_deg_1();

    computeOnDegree();

    computeConnexions();

    sample();

    make();

    //Object::rotate(90, {1, 0, 0});
}

SPatch::~SPatch()
{

}

// gérère le premier ensemble de points du polygone -> polygone de base
void SPatch::gen_deg_1()
{
    for (uint i = 0; i < m_size; i++)
    {
        float x = glm::cos(M_PI * 2 * (static_cast<float>(i) / static_cast<float>(m_size)));
        float y = glm::sin(M_PI * 2 * (static_cast<float>(i) / static_cast<float>(m_size)));
        m_cp.push_back(ControlPoint(glm::vec3(x, y, 0), glm::vec2(x,y), Index(m_size, i, 1)));
        m_base_poly.push_back(ControlPoint(glm::vec3(x, y, 0), glm::vec2(x,y), Index(m_size, i, 1)));
    }
}

// sortir de la class
// récupère un CP dans une liste à partir de son index
ControlPoint getControlPointByIndex(Index index, std::vector<ControlPoint> cps)
{
    for (uint i = 0; i < cps.size(); i++)
    {
        bool found = true;
        for (uint j = 0; j < index.size(); j++)
        {
            if (cps[i].index()[j] != index[j])
            {
                found = false;
            }
        }
        if (found)
            return cps[i];
    }
    std::cout << "not found" << std::endl;
    return ControlPoint({0, 0, 0}, {0,0}, Index(index.size()));
}

// génère toutes les distribution possibles d'un vecteur d'indices (d'entier) pour un degré (une norme) donné.e
void SPatch::generateDistribution(std::vector<Index> *distributions, Index index, uint degree, uint id)
{
    if (id == index.size() - 1)
    {
        index[id] = degree;
        distributions->push_back(index);
    }
    else
    {
        for (uint i = 0; i <= degree; i++)
        {
            Index index_copy = index;
            index_copy[id] = i;
            generateDistribution(distributions, index_copy, degree - i, id + 1);
        }
    }
}

// génère les CP de degré supérieur à 1
void SPatch::computeOnDegree()
{
    for (uint i = 2; i <= m_degree; i++)
    {
        std::vector<ControlPoint> m_cp2;

        std::vector<Index> *distributions = new std::vector<Index>;
        generateDistribution(distributions, Index(m_size, 0), i, 0);

        for (Index index : *(distributions))
        {
    glm::vec3 new_coord(0, 0, 0);
    for (uint i = 0; i < index.size(); i++)
    {
        if (index[i] > 0)
        {
            Index index_deg_min_1(index);
            index_deg_min_1[i] = index_deg_min_1[i] - 1;

            ControlPoint cp_deg_min_1 = getControlPointByIndex(index_deg_min_1, m_cp);

            float cp_weight = static_cast<float>(index[i]) / static_cast<float>(m_degree);

            new_coord.x += cp_weight * cp_deg_min_1.position().x;
            new_coord.y += cp_weight * cp_deg_min_1.position().y;
        }
    }
    new_coord.z = static_cast<float>(std::pow(new_coord.x, 2) + std::pow(new_coord.y, 2));

            //m_cp2.push_back(ControlPoint(computeNewCoord(d), d));
            m_cp2.push_back(ControlPoint(new_coord, {0,0},index));
        }
        m_cp = m_cp2;
    }
}

// règle de connexion des points de controles pour créer le maillage de controle
// voir publication
void SPatch::computeConnexions()
{
    for (uint l = 0; l < m_cp.size(); l++)
    {
        for (uint h = 0; h < m_cp.size(); h++)
        {
            bool index_exists = false;
            for (uint j = 0; j < m_size; j++)
            {
                if (m_cp[l].index() != m_cp[h].index() && m_cp[l].index().degree() == m_degree && m_cp[h].index().degree() == m_degree)
                {
                    bool cond1 = true;
                    uint j_mod = j % m_size;
                    uint j1_mod = (j + 1) % m_size;

                    for (uint i = 0; i < m_size; i++)
                    {
                        // conditions check
                        if ((m_cp[l].index()[i] != m_cp[h].index()[i] && (i != j_mod) && (i != j1_mod)))
                        {
                            cond1 = false;
                        }
                    }
                    if (cond1)
                    {
                        if ((m_cp[l].index()[j1_mod] - 1) == m_cp[h].index()[j1_mod])
                        {
                            if (m_cp[l].index()[j_mod] + 1 == m_cp[h].index()[j_mod])
                            {
                                index_exists = true;
                            }
                        }
                    }
                }
            }
            if (index_exists)
            {
                // push m_cp[l] && m_cp[h] in vbo
                m_indices_connex.push_back(l);
                m_indices_connex.push_back(h);
            }
        }
    }
}

// calcule les coordonnées 3D d'un point en fonction de ses coord MVC
glm::vec3 SPatch::evaluateFromMVC(std::vector<double> MVC)
{
    std::vector<ControlPoint> pts = m_cp;

    for (int i = static_cast<int>(m_degree)-1; i >= 0; i--)
    {
        std::vector<Index> *distributions = new std::vector<Index>;
        generateDistribution(distributions, Index(m_size, 0), static_cast<uint>(i), 0);

        for (uint j = 0; j < distributions->size(); j++)
        {
            std::vector<ControlPoint> parents = getParentCPs(distributions->at(j), pts);

            glm::vec3 new_coords(0,0,0);
            for (uint i = 0; i < m_size; i++)
            {
                new_coords += parents[i].position() * static_cast<float>(MVC[i]);
            }
            pts[j] = ControlPoint(new_coords, {0,0}, distributions->at(j));
        }
    }
    return pts[0].position();
}

// obselette
// ne considère pas les cas aux bords
/*
std::vector<float> SPatch::computeMVC(glm::vec3 point, std::vector<ControlPoint> neighbours)
{
    std::vector<float> weights;
    float sum = 0.0f;

    for (uint i = 0; i < m_size; i++)
    {
        glm::vec3 currentN = neighbours[i % m_size].position();
        glm::vec3 nextN = neighbours[(i + 1) % m_size].position();
        glm::vec3 prevN = neighbours[i == 0 ? m_size - 1 : i - 1].position();

        glm::vec3 currentV = currentN - point;
        glm::vec3 prevV = prevN - point;
        glm::vec3 nextV = nextN - point;

        float prevAngle = glm::angle(prevV, currentV);
        float nextAngle = glm::angle(currentV, nextV);

        float currentW = (std::tan(prevAngle / 2.0f) + std::tan(nextAngle / 2.0f)) / glm::length(currentV);

        weights.push_back(currentW);
        sum += currentW;
    }

    for (uint i = 0; i < weights.size(); i++)
    {
        weights[i] /= sum;
    }
    return weights;
}
*/

// compute MVC of a point from its 2D-parametric coordinates
std::vector<double> SPatch::computeMVC(glm::vec3 point, std::vector<ControlPoint> cp)
{
    //std::cout << glm::to_string(point) << std::endl;
    std::vector<double> weights;
    double sum = 0.0;
    double MVCProduct;

    for (uint i = 0; i < cp.size(); i++)
    {
        uint id_prev = (i <= 0 ? m_size - 1 : i - 1);
        uint id_curr = (i % m_size);
        uint id_next = ((i + 1) % m_size);

        MVCProduct = 1.0;

        glm::vec3 vi_prev = cp[id_prev].position();
        glm::vec3 vi_next = cp[id_next].position();

        glm::vec3 di_prev = vi_prev - point;
        glm::vec3 di_next = vi_next - point;

        double ri_prev = static_cast<double>(glm::length(di_prev));
        double ri_next = static_cast<double>(glm::length(di_next));

        for (uint j = 0; j < m_size; j++)
        {
            if (j != id_curr && j != id_prev)
            {
                glm::vec3 vj_curr = cp[j % m_size].position();
                glm::vec3 vj_next = cp[(j + 1) % m_size].position();

                glm::vec3 dj_curr = vj_curr - point;
                glm::vec3 dj_next = vj_next - point;
                double rj_curr = static_cast<double>(glm::length(dj_curr));
                double rj_next = static_cast<double>(glm::length(dj_next));

                double value = std::sqrt((rj_curr * rj_next) + (static_cast<double>(glm::dot(dj_curr, dj_next))));
                value = (value > 0 ? value : 0);

                MVCProduct *= value;
            }
        }

        double current_weight = std::sqrt((ri_prev * ri_next) - static_cast<double>(glm::dot(di_prev, di_next))) * MVCProduct;
        weights.push_back(current_weight);
        sum += current_weight;
    }

    for (uint i = 0; i < weights.size(); i++)
    {
        weights[i] /= sum;
    }
    return weights;
}

// retrieves parents of control points
std::vector<ControlPoint> SPatch::getParentCPs(Index index, std::vector<ControlPoint> cps)
{
    std::vector<ControlPoint> parents;
    for (uint i = 0; i < m_size; i++)
    {
        Index current = index;
        current[i]++;
        parents.push_back(getControlPointByIndex(current, cps));
    }
    return parents;
}

// Sample the whole polygon
void SPatch::sample()
{
    glm::vec3 O(0,0,0);
    /*for (ControlPoint cp : m_base_poly)
        O += cp.position() / (float)(m_size);*/

    // pour chaque triganle formé depuis le centre avec les paires de sommet en bordure
    for (uint i = 0; i < m_base_poly.size(); i++)
    {
        // on echanillonne le triagnle
        sampleTriangle(O, m_base_poly[i].position(), m_base_poly[(i + 1) % m_size].position());
        
        // on créer les indices opengl
        genIndicesList();
    }
}

// sample a unique triangle
// warning : generate duplicate vertices on triangle edges
void SPatch::sampleTriangle(glm::vec2 O, glm::vec2 A, glm::vec2 B)
{
    //std::cout << "O:" << glm::to_string(O) << " A:" << glm::to_string(A) << " B:" << glm::to_string(B) << std::endl;

    // initial bottom row
    for (uint i = 0; i <= m_samplingRate; i++)
    {
        glm::vec2 sp = A + (B - A) * (static_cast<float>(i) / static_cast<float>(m_samplingRate));
        m_surfaceMVC.push_back(computeMVC(glm::vec3(sp.x, sp.y, 0), m_base_poly));
    }

    // upper rows
    for (uint i = 1; i <= m_samplingRate; i++)
    {
        glm::vec2 rs = A + (O - A) * (static_cast<float>(i) / static_cast<float>(m_samplingRate)); // row start
        glm::vec2 re = B + (O - B) * (static_cast<float>(i) / static_cast<float>(m_samplingRate)); // row end
        if (m_samplingRate - i != 0)
        {
            for (uint j = 0; j <= m_samplingRate - i; j++)
            {
                glm::vec2 sp = rs + (re - rs) * (static_cast<float>(i) / static_cast<float>(m_samplingRate-i));
                m_surfaceMVC.push_back(computeMVC(glm::vec3(sp.x, sp.y, 0), m_base_poly));
            }

        }
        else // top of the triangle (= center of the polygon)
        {
            m_surfaceMVC.push_back(computeMVC(glm::vec3(O.x, O.y, 0), m_base_poly));
        }
    }
}

// génère la liste des indices des points pour OpenGL (forme les triangles)
void SPatch::genIndicesList()
{
    uint n = 0;
    if (!m_surface_indices.empty())
    {
        n = *std::max_element(m_surface_indices.begin(), m_surface_indices.end()) + 1;
    };
    uint l = m_samplingRate;

    // per row
    for (uint i = 0; i < m_samplingRate; i++)
    {
        l = m_samplingRate - i;
        uint nbtri = m_samplingRate * 2 - 1 - i * 2;
        uint k = 0;
        // per triangle in row
        for (uint j = 0; j < nbtri; j++)
        {
            // if upward triangle
            if (j % 2 == 0)
            {
                m_surface_indices.push_back(n + k);
                m_surface_indices.push_back(n + l + 1 + k);
                m_surface_indices.push_back(n + 1 + k);
            }
            // if downward
            else //j % 2 == 1
            {
                k++;
                m_surface_indices.push_back(n + k);
                m_surface_indices.push_back(n + l + k);
                m_surface_indices.push_back(n + l + 1 + k);
            }
        }
        n = n + l + 1;
    }
}

int SPatch::pickCP(glm::vec3 d, glm::vec3 p)
{
    typedef struct dist_cp{
        double t;
        std::size_t i;
        ControlPoint cp;
    } DCP; // distance + CP

    std::vector<DCP> hit_cp;
    std::size_t i = 0;
    for( auto cp : m_cp)
    {
        double *t = cp.intersect(p, d);
        if(t != nullptr)
        {
            hit_cp.push_back({*t,i,cp});
        }
        i++;
    }

    std::sort(hit_cp.begin(), hit_cp.end(), [](const DCP &a, const DCP &b)->bool{return a.t < b.t;});

    if(hit_cp.size()>0)
        return static_cast<int>(hit_cp.at(0).i);
    else
        return -1;
}

void SPatch::moveSelectedCP(glm::vec3 t)
{
    for(std::size_t i = 0 ; i < m_selectedCPs.size();i++)
    {
        m_cp.at(m_selectedCPs.at(i)).position(m_cp.at(m_selectedCPs.at(i)).position()+t);
    }
    updateCPBuffer();
    updateSurfaceBuffer();
}

void SPatch::toggleDrawCP()
{
    m_drawCP = !m_drawCP;
}

const std::vector<ControlPoint>& SPatch::controlPoints()
{
    return m_cp;
}

void SPatch::addToSelection(int i)
{
    if(!find(m_selectedCPs,static_cast<std::size_t>(i)))
    {
        m_selectedCPs.push_back(static_cast<std::size_t>(i));
    }
    updateCPBuffer();
}

void SPatch::selectedCP(std::size_t select) {
    m_selectedCPs.clear();
    m_selectedCPs.push_back(select);
    updateCPBuffer();
}

// créer et remplis les array OpenGL
void SPatch::make()
{

    // #################" CONTROL POINTS #####################

    // CP buffer initialisation
    glGenVertexArrays(1, &m_vao_cp);
    glGenBuffers(1, &m_vbo_cp);
    glGenBuffers(1, &m_ebo_l);

    updateCPBuffer();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo_l);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices_connex.size() * sizeof(unsigned int), m_indices_connex.data(), GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    // vertex color
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));

    //glBindVertexArray(0);

    // ######################### SURFACE ################################

    // surface buffer initialisation
    glGenVertexArrays(1, &m_vao_surf);
    glGenBuffers(1, &m_vbo_surf);
    //glGenBuffers(1, &m_ebo_surf);

    updateSurfaceBuffer();

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    // vertex color
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));

    glBindVertexArray(0);
}

void SPatch::updateCPBuffer()
{
    std::vector<Vertex> cpv; // equivalent vertices of control points

    std::size_t i = 0;
    glm::vec3 color;
    for (ControlPoint cp : m_cp)
    {
        if(find(m_selectedCPs,i))
        {
            color = {1,1,0};
        }
        else
        {
            color = {1,0,0};
        }
        cpv.push_back({cp.position(), {0, 1, 0}, color});
        i++;
    }
    glBindVertexArray(m_vao_cp);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_cp);
    glBufferData(GL_ARRAY_BUFFER, cpv.size() * sizeof(Vertex), cpv.data(), GL_STATIC_DRAW);
}

void SPatch::updateSurfaceBuffer()
{
    // on évalue tout les points uniques de la surface d'apres les MVC
    std::vector<glm::vec3> surface_points;
    //int i = 0;
    for (std::vector<double> p : m_surfaceMVC)
    {
        glm::vec3 pe = evaluateFromMVC(p);
        surface_points.push_back(pe);
    }
// ##############

    // on créer les triangles d'apres des les indices
    std::vector<Vertex> surface_complete;
    for(uint i = 0; i < m_surface_indices.size(); i+=3)
    {

        uint i0 = m_surface_indices[i];   // indice premier point du triangle
        uint i1 = m_surface_indices[i+1]; // indice second point
        uint i2 = m_surface_indices[i+2]; // indice troisième point

        glm::vec3 p0 = surface_points[i0]; // 1er point du triangle
        glm::vec3 p1 = surface_points[i1]; // 2nd
        glm::vec3 p2 = surface_points[i2]; // 3e

        glm::vec3 normal = (glm::cross(p1 - p0, p2 - p0));
        //std::cout << glm::to_string(normal)<<std::endl;

        surface_complete.push_back({p0,normal});
        surface_complete.push_back({p1,normal});
        surface_complete.push_back({p2,normal});
    }

    glBindVertexArray(m_vao_surf);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_surf);
    glBufferData(GL_ARRAY_BUFFER, surface_complete.size() * sizeof(Vertex), surface_complete.data(), GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo_surf);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_surface_indices.size() * sizeof(unsigned int), m_surface_indices.data(), GL_STATIC_DRAW);
}

// déssine le SPatch
void SPatch::draw(Shader shader)
{   
    shader.bind();
    shader.setUniformBool("useVColor", false);

    // draw surface
    glBindVertexArray(m_vao_surf);
    glDrawArrays(GL_TRIANGLES, 0, m_surface_indices.size());
    //glDrawElements(GL_POINTS, m_surface_indices.size(), GL_UNSIGNED_INT, 0);

    if(m_drawCP)
    {
        shader.setUniformBool("useVColor", true);

        // draw CPs
        glBindVertexArray(m_vao_cp);
        glDrawArrays(GL_POINTS, 0, m_cp.size());

        // draw CP connections
        glDrawElements(GL_LINES, m_indices_connex.size(), GL_UNSIGNED_INT, 0);
    }
}
