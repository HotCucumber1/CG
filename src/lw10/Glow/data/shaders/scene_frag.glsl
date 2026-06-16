#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform bool isGlowing;
uniform bool renderMask;

void main() {
    if (renderMask) {
        FragColor = isGlowing
            ? vec4(objectColor, 1.0)
            : vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        if (isGlowing) {
            FragColor = vec4(objectColor, 1.0);
        } else {
            vec3 lightDir = normalize(vec3(0.0, 0.0, 0.0) - FragPos);
            float diff = max(dot(normalize(Normal), lightDir), 0.0);
            vec3 ambient = 0.1 * objectColor;
            FragColor = vec4(ambient + diff * objectColor, 1.0);
        }
    }
}