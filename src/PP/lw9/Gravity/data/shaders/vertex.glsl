#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in float mass;

uniform mat4 modelViewProjection;
uniform float pointSize;

void main()
{
    float dist = distance(cameraPos, position);
    gl_PointSize = 20.0 / (dist * 0.1 + 0.5);
    gl_Position = modelViewProjection * vec4(position, 1.0);
}
