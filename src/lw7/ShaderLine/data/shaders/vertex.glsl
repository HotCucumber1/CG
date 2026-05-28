#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 uProjection;

out float vAngle;

void main()
{
    float x = aPos.x;
    float r = (1 + sin(x)) *
              (1 + 0.9 * cos(8 * x)) *
              (1 + 0.1 * cos(24 * x)) *
              (0.5 + 0.05 * cos(140 * x));

    float polarX = r * cos(x);
    float polarY = r * sin(x);
    float polarZ = aPos.z;

    gl_Position = uProjection * vec4(polarX, polarY, polarZ, 1.0);

    vAngle = x;
}
