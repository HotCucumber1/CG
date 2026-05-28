#version 330 core
out vec4 FragColor;

in float vAngle;

void main()
{
    float t = vAngle / (2.0 * 3.141592);

    vec3 greenColor = vec3(0.0, 1.0, 0.0);
    vec3 redColor = vec3(1.0, 0.0, 0.0);

    vec3 color = mix(greenColor, redColor, t);

    FragColor = vec4(color, 1.0);
}

