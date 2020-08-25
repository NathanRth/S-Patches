#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
out vec3 Color;

//out vec4 FragPos;

void main()
{
    gl_PointSize = 10.0;
    gl_Position = projection * view * vec4(position, 1.0);
    FragPos = position;

    mat3 normalMatrix = transpose(inverse(mat3(view * model)));
    Normal = normalize(vec3(normal.xy, normal.z));
    Color = color;
};