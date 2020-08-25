#version 330 core

uniform vec3 color;

uniform bool useVColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 Color;

out vec4 FragColor;

void main()
{
    vec3 lightPos = vec3(1,1,0);
    vec3 lightDir = lightPos - FragPos;
    float shade = max(dot(Normal,lightDir),0.2);
    if(useVColor)
    {
        FragColor = vec4(Color, 1.0f);
    }
    else
    {
        FragColor = vec4(shade, shade, shade, 1.0f);
    }
}
