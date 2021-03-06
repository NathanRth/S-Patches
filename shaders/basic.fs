#version 330 core

out vec4 FragColor;

in vec2 UVs;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;

void main()
{             
    FragColor = texture(gAlbedo, UVs);
}
