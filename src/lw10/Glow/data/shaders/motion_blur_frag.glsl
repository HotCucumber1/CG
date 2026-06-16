#version 330 core

out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D currentGlow;
uniform sampler2D previousGlow;
uniform float decay;

void main()
{
    vec4 current = texture(currentGlow, TexCoords);
    vec4 previous = texture(previousGlow, TexCoords);
    FragColor = current + previous * decay;
}