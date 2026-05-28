#version 330 core
out vec4 color;

uniform vec3 particleColor;

void main()
{
    vec2 coord = gl_PointCoord;
    float dist = length(coord - vec2(0.5));
    if (dist > 0.5) discard;

    float brightness = 1.0 - smoothstep(0.0, 0.5, dist);
    color = vec4(particleColor, brightness * 0.8);
}