#version 330 core
in vec2 vTexCoord;
out vec4 FragColor;
uniform sampler2D uTexture;
uniform float uAngleRad;
uniform vec2 uTexelSize;

float luminance(vec3 color)
{
    return dot(color, vec3(0.299, 0.587, 0.114));
}

void main()
{
    vec3 centerColor = texture(uTexture, vTexCoord).rgb;

    float tl = luminance(texture(uTexture, vTexCoord + vec2(-uTexelSize.x,  uTexelSize.y)).rgb);
    float tm = luminance(texture(uTexture, vTexCoord + vec2( 0.0, uTexelSize.y)).rgb);
    float tr = luminance(texture(uTexture, vTexCoord + vec2( uTexelSize.x,  uTexelSize.y)).rgb);
    float ml = luminance(texture(uTexture, vTexCoord + vec2(-uTexelSize.x,  0.0)).rgb);
    float mr = luminance(texture(uTexture, vTexCoord + vec2( uTexelSize.x,  0.0)).rgb);
    float bl = luminance(texture(uTexture, vTexCoord + vec2(-uTexelSize.x, -uTexelSize.y)).rgb);
    float bm = luminance(texture(uTexture, vTexCoord + vec2( 0.0, -uTexelSize.y)).rgb);
    float br = luminance(texture(uTexture, vTexCoord + vec2( uTexelSize.x, -uTexelSize.y)).rgb);

    float Gx = (tr + 2.0 * mr + br) - (tl + 2.0 * ml + bl);
    float Gy = (tl + 2.0 * tm + tr) - (bl + 2.0 * bm + br);

    float edge = abs(Gx * cos(uAngleRad) + Gy * sin(uAngleRad));
    edge = clamp(edge * 0.25, 0.0, 1.0);

    FragColor = vec4(edge, edge, edge, 1.0);
}
