#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform vec2 uResolution;

#define M_PI 3.141592

mat2 Rotate2d(float angle)
{
    return mat2(cos(angle), -sin(angle),
                sin(angle),  cos(angle));
}

bool IsInsideStar(vec2 p, vec2 center, float r, float angle)
{
    vec2 localCenter = p - center;
    localCenter = Rotate2d(-angle) * localCenter;
    float l = length(localCenter);
    float a = atan(localCenter.y, localCenter.x) + M_PI / 2.0;

    float startRay = 2.0 * M_PI / 5.0;

    float f = mod(a, startRay) - startRay / 2.0;
    f = abs(f);

    vec2 baseCoord = l * vec2(cos(f), sin(f));

    vec2 rA = vec2(r, 0.0);
    vec2 rB = r * 0.38196 * vec2(0.809017, 0.587785);
    float det = (baseCoord.x - rA.x) * (rB.y - rA.y) - (baseCoord.y - rA.y) * (rB.x - rA.x);

    return det < 0.0;
}

void main()
{
    float windowAspect = uResolution.x / uResolution.y;
    float flagAspect = 1.5;
    vec2 uv = TexCoords;

    if (windowAspect > flagAspect)
    {
        float s = flagAspect / windowAspect;
        uv.x = (uv.x - 0.5) / s + 0.5;
    } 
    else 
    {
        float s = windowAspect / flagAspect;
        uv.y = (uv.y - 0.5) / s + 0.5;
    }

    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0)
    {
        FragColor = vec4(0.12, 0.12, 0.14, 1.0);
        return;
    }

    vec2 p = uv;
    p.x *= 1.5;

    vec3 redColor  = vec3(0.87, 0.14, 0.14);
    vec3 goldColor = vec3(1.0, 0.86, 0.0);
    vec3 finalColor = redColor;

    vec2 centerBig = vec2(0.25, 0.75);
    float radiusBig = 0.15;

    bool drawGold = IsInsideStar(p, centerBig, radiusBig, 0.0);

    vec2 smallCenters[4];
    smallCenters[0] = vec2(0.5, 0.9);
    smallCenters[1] = vec2(0.6, 0.8);
    smallCenters[2] = vec2(0.6, 0.65);
    smallCenters[3] = vec2(0.5, 0.55);

    float radiusSmall = 0.05;

    float smallAngles[4];
    smallAngles[0] = atan(centerBig.y - smallCenters[0].y, centerBig.x - smallCenters[0].x) - M_PI / 2.0;
    smallAngles[1] = atan(centerBig.y - smallCenters[1].y, centerBig.x - smallCenters[1].x) - M_PI / 2.0;
    smallAngles[2] = atan(centerBig.y - smallCenters[2].y, centerBig.x - smallCenters[2].x) - M_PI / 2.0;
    smallAngles[3] = atan(centerBig.y - smallCenters[3].y, centerBig.x - smallCenters[3].x) - M_PI / 2.0;

    for (int i = 0; i < 4; i++)
    {
        if (IsInsideStar(p, smallCenters[i], radiusSmall, smallAngles[i]))
        {
            drawGold = true;
        }
    }

    if (drawGold)
    {
        finalColor = goldColor;
    }

    FragColor = vec4(finalColor, 1.0);
}
