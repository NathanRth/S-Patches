#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "bezier_curve.hpp"
#include "s_patch.hpp"
#include "camera.hpp"

/**
 * @brief Une scène sert à décrire un environement 3D.
 * Elle contient les différents objets et lumières présents
 * dans le monde 3D.
 */

class Scene
{
  public:
    Scene();

    Camera *camera() { return m_camera; }

    /**
     * @brief Ajouter une courbe de bezier dans la scène
     * @param Le model à ajouter \see Model
     */
    void addBezierCurve(const BezierCurve &curve);

    void addSPatch(const SPatch &spatch);

    /**
     * @brief Ajouter une lumière dans la scène.
     * @param light La lumière à ajouter. La lumière doit être de type \c PointLight* ou \c DirectionalLight*
     */
    //void addLight(Light* light);

    /**
     * @brief Supprimer une courbe de bezier
     * @param Index de l'objet dans la liste
     */
    void removeBezierCurve(unsigned int index);

    /**
     * @brief Supprimer une lumière
     * @param Index de la lumière dans la liste
     */
    //void removeLight(unsigned int index);

    std::vector<BezierCurve> *curves() { return &m_curves; } /**< Renvoi la liste des objets de la scène */
    std::vector<SPatch> *spatches() { return &m_spatches; }
    //std::vector<Light*>* lights() {return &m_lights;} /**< Renvoi la liste des lumière de la scène */

    void drawAxis(Shader shader);

  private:
    std::vector<SPatch> m_spatches;
    std::vector<BezierCurve> m_curves; /**< Liste des model de la scène */
    //std::vector<Light*> m_lights; /**< Liste des lumière de la scène. Exprimée sous la forme
    //                                d'une liste de pointeur sur la classe abstraite Light */
    Camera *m_camera; /**< La camera de la scène */

    GLuint m_axis_vao, m_axis_vbo, m_axis_ebo;
};

#endif // SCENE_H
