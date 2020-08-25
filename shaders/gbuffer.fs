#version 330 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out float gDepth;

in vec4 FragPos;
in vec3 Normal;

const float FAR = 100.0;
const float NEAR = 0.1;

float linearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));
}

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition.rgb = FragPos.xyz;
    float ld = linearizeDepth(gl_FragCoord.z);
    gPosition.a = ld;
    gDepth = ld;
    gNormal = normalize(Normal);
    gAlbedo = vec3(1.0, 1.0, 1.0);
}
