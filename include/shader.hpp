#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <ostream>

class Shader
{
  public:
    Shader(std::string vertexPath, std::string fragmentPath);

    void bind();

    void setSamplers(std::vector<std::string> samplers);
    void setUniformFloat(std::string name, float val);
    void setUniformInt(std::string name, int val);
    void setUniformUInt(std::string name, unsigned int val);
    void setUniformBool(std::string name, bool val);
    void setUniformVec2(std::string name, glm::vec2 val);
    void setUniformVec3(std::string name, glm::vec3 val);
    void setUniformMat3(std::string name, glm::mat3 val);
    void setUniformVec4(std::string name, glm::vec4 val);
    void setUniformMat4(std::string name, glm::mat4 val);

    friend std::ostream &operator<<(std::ostream &o, const Shader &s);

  private:
    unsigned int m_id;
    std::string vs;
    std::string fs;
    unsigned int m_nbSampler;
};

#endif // SHADER_H
