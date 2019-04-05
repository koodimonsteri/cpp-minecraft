#version 330

in vec3 myVertexPos;
in vec3 myVertexNormal;
in vec2 uv;
flat in int passTexId;
in mat2 aoTable;

struct Fog {
    int    active;
    float  intensity;
    vec3   color;
};

uniform sampler2DArray my_sampler;
uniform vec3           ambientLight;
uniform Fog            fog;

out vec4 fragmentColor;

vec4 calculateFog(vec3 pos, vec4 color, Fog f, vec3 ambient)
{
    vec3 fogColor = f.color * ambientLight;
    float dist = length(pos);
    float fogF = 1.0 / exp((dist * f.intensity));
    fogF = clamp(fogF, 0.0, 1.0);
    vec3 resultColour = mix(fogColor, color.xyz, fogF);

    return vec4(resultColour.xyz, color.w);
}

void main()
{
    float ao = dot(vec2(1 - uv.x, uv.x) * aoTable, vec2(1 - uv.y, uv.y));
    ao *= ao;
    ao *= ao;

    fragmentColor = texture(my_sampler, vec3(uv, passTexId)) * (1 - ao)* vec4(ambientLight, 1.0);

    fragmentColor *= mod(myVertexNormal.y, 1.0);
    if(fog.active == 1)
    {
        fragmentColor = calculateFog(myVertexPos, fragmentColor, fog, ambientLight);
    }
    fragmentColor.w = 0.6;
}