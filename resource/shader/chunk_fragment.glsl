#version 330

// IN
in mat2 aoTable;
in vec2 uv;
flat in int passTexId;
in vec3 myVertexPos;

struct Fog {
    int    active;
    float  intensity;
    vec3   color;
};

// UNIFORMS
uniform sampler2DArray my_sampler;
uniform vec3           ambientLight;
uniform Fog            fog;

// OUT
out vec4 outFragColor;

vec4 calculateFog(vec3 pos, vec4 color, Fog fog, vec3 ambient)
{
    vec3 fogColor = fog.color * ambientLight;
    float dist = length(pos);
    float fogF = 1.0 / exp((dist * fog.intensity));
    fogF = clamp(fogF, 0.0, 1.0);
    vec3 resultColour = mix(fogColor, color.xyz, fogF);

    return vec4(resultColour.xyz, color.w);
}

void main() {
    // Get luminosity using bilinear interpolation
    float ao = dot(vec2(1 - uv.x, uv.x) * aoTable, vec2(1 - uv.y, uv.y));
    ao *= ao;
    ao *= ao;

    outFragColor = texture(my_sampler, vec3(uv, passTexId)) * (1 - ao) * vec4(ambientLight, 1.0);

    if(fog.active == 1)
    {
        outFragColor = calculateFog(myVertexPos, outFragColor, fog, ambientLight);
    }
}