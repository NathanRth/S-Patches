#version 330 core

out vec4 FragColor;

uniform sampler2D gAlbedo;

void main()
{             
    // retrieve data from gbuffer
    vec3 Albedo = texture(gAlbedo, TexCoords).rgb;

    FragColor = vec4(Albedo,1.0);
}