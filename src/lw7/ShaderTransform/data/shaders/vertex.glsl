#version 330 core
layout (location = 0) in vec2 aParam;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform float uMorphFactor;

out vec3 FragPos;
out vec3 Normal;

const float PI = 3.14159265359;

// TODO исправить
vec3 GetMobius(float u, float v)
{
    float vScaled = (v / PI - 1.0) * 1.2;
    float R = 3.0;

    float x = (R + vScaled * cos(u * 0.5)) * cos(u);
    float y = (R + vScaled * cos(u * 0.5)) * sin(u);
    float z = vScaled * sin(u * 0.5);

    return vec3(x, y, z);
}

vec3 GetKlein(float u, float v)
{
    float R = 3.0;

    float x = (R + cos(u * 0.5) * sin(v) - sin(u * 0.5) * sin(2.0 * v)) * cos(u);
    float y = (R + cos(u * 0.5) * sin(v) - sin(u * 0.5) * sin(2.0 * v)) * sin(u);
    float z = sin(u * 0.5) * sin(v) + cos(u * 0.5) * sin(2.0 * v);

    return vec3(x, y, z);
}

vec3 GetMorphedPosition(float u, float v)
{
    vec3 mobius = GetMobius(u, v);
    vec3 klein = GetKlein(u, v);
    return mix(mobius, klein, uMorphFactor);
}

void main()
{
    float u = aParam.x;
    float v = aParam.y;

    vec3 morphedPos = GetMorphedPosition(u, v);

    float eps = 0.005;
    vec3 posU = GetMorphedPosition(u + eps, v);
    vec3 posV = GetMorphedPosition(u, v + eps);

    vec3 tangentU = posU - morphedPos;
    vec3 tangentV = posV - morphedPos;

    vec3 rawNormal = cross(tangentU, tangentV);

    FragPos = vec3(uModel * vec4(morphedPos, 1.0));
    Normal = normalize(mat3(transpose(inverse(uModel))) * rawNormal);

    gl_Position = uProjection * uView * vec4(FragPos, 1.0);
}
