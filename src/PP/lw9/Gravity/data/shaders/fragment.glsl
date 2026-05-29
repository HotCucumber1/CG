#version 330 core
out vec4 color;

uniform vec3 particleColor;

void main()
{
    color = vec4(particleColor, 1.0);
}
