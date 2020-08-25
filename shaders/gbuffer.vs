#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec4 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_PointSize = 12.0;
    
    vec4 viewPos = view * model * vec4(position, 1.0);
    FragPos = viewPos;
    
    mat3 normalMatrix = transpose(inverse(mat3(view * model)));
    Normal = normalMatrix * normal;

    gl_Position = projection * viewPos;
}