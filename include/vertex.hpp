#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

typedef struct s_vertex
{
    s_vertex(glm::vec3 p, glm::vec3 n, glm::vec3 c = glm::vec3(1,0,1)){position = p; normal = n; color = c;}
    glm::vec3 position; /**< Position du vertex */
    glm::vec3 normal; /**< Normal du vertex */
    glm::vec3 color; /** Couleur du vertex */
    //glm::vec2 uv; /**< Position UV du vertex */
    //glm::vec3 tangent; /**< Tengente du vertex, calculée par Assimp lors de l'importation */
} Vertex;

#endif // VERTEX_H
