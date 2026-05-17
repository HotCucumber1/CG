#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 uLightPos;
uniform vec3 uViewPos;

void main()
{
    vec3 ambientColor = vec3(0.12, 0.12, 0.16);
    vec3 diffuseColor = vec3(0.25, 0.65, 0.95);

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(uLightPos - FragPos);

    float diff = abs(dot(norm, lightDir));
    vec3 diffuse = diff * diffuseColor;

    vec3 finalColor = ambientColor + diffuse;

    FragColor = vec4(finalColor, 1.0);
}