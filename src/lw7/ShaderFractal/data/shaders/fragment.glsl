#version 400 core
out vec4 FragColor;
in vec2 TexCoords;

uniform dvec2 uOffset;
uniform double uZoom;
uniform vec2 uResolution;

void main()
{
    double aspect = double(uResolution.x / uResolution.y);

    dvec2 c;
    c.x = (double(TexCoords.x) - 0.5) * aspect * uZoom + uOffset.x;
    c.y = (double(TexCoords.y) - 0.5) * uZoom + uOffset.y;

    dvec2 z = dvec2(0.0, 0.0);

    int maxIterations = 256;
    int iter = 0;

    while (z.x * z.x + z.y * z.y <= 4.0 && iter < maxIterations)
    {
        double x_temp = z.x * z.x - z.y * z.y + c.x;
        z.y = 2.0 * z.x * z.y + c.y;
        z.x = x_temp;
        iter++;
    }

    if (iter == maxIterations)
    {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    float log_zn = log(float(z.x * z.x + z.y * z.y)) / 2.0;
    float nu = log(log_zn / log(2.0)) / log(2.0);
    float t = (float(iter) + 1.0 - nu) / float(maxIterations);

    vec3 color;
    color.r = 0.5 + 0.5 * cos(3.0 + t * 20.0 + 0.0);
    color.g = 0.5 + 0.5 * cos(3.0 + t * 20.0 + 0.6);
    color.b = 0.5 + 0.5 * cos(3.0 + t * 20.0 + 1.0);

    FragColor = vec4(color, 1.0);
}
