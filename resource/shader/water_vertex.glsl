#version 330

layout (location = 0) in uvec3 blockPos;
layout (location = 1) in int texId;
layout (location = 2) in int faceData;

uniform mat4  projectionMatrix;
uniform mat4  modelViewMatrix;
uniform float deltatime;

out vec3 myVertexPos;
out vec3 myVertexNormal;
out vec2 uv;
flat out int passTexId;
out mat2 aoTable;

uniform vec2 uvs[4] = vec2[4]( vec2(0,1), vec2(0,0), vec2(1,1), vec2(1,0) );

uniform vec3 positions[24] = vec3[24](
    vec3(1, 1, 1), vec3(1, 0, 1), vec3(1, 1, 0), vec3(1, 0, 0), // X_POS
    vec3(0, 1, 0), vec3(0, 0, 0), vec3(0, 1, 1), vec3(0, 0, 1), // X_NEG
    vec3(0, 1, 0), vec3(0, 1, 1), vec3(1, 1, 0), vec3(1, 1, 1), // Y_POS
    vec3(1, 0, 0), vec3(1, 0, 1), vec3(0, 0, 0), vec3(0, 0, 1), // Y_NEG
    vec3(0, 1, 1), vec3(0, 0, 1), vec3(1, 1, 1), vec3(1, 0, 1), // Z_POS
    vec3(1, 1, 0), vec3(1, 0, 0), vec3(0, 1, 0), vec3(0, 0, 0)  // Z_NEG
);

void createAoTable() {
    float oclusion = 0.7;
    mat2 values;
    values[1][0] = (faceData & (1 << 4)) != 0 ? oclusion : 0.0;
    values[1][1] = (faceData & (2 << 4)) != 0 ? oclusion : 0.0;
    values[0][0] = (faceData & (4 << 4)) != 0 ? oclusion : 0.0;
    values[0][1] = (faceData & (8 << 4)) != 0 ? oclusion : 0.0;
    aoTable = values;
}

vec4 lerpPos(float dt, vec4 pos)
{
    vec4 res = pos;
    float pi = 3.14159265359;

    float tempx, tempz;
    tempx = sin((pi / pow(2, 2))*pos.x);
    tempz = sin((pi / pow(2, 3))*pos.z);
    res.y += sin((dt+tempx))/20 + cos((dt+tempz))/20 + sin(dt+tempz+tempx)/15 + cos(dt+tempz-tempx)/15 - 0.3;
    return res;
}

void main()
{
    createAoTable();
    passTexId = texId;
    uv = uvs[gl_VertexID % 4];

    vec4 pos2 = vec4(positions[4*(faceData & 7) + gl_VertexID % 4] + blockPos, 1);
    pos2 = lerpPos(deltatime, pos2);

    // Not a real normal!! just a hack to give water slight color differences for now
    vec3 tempNorm = pos2.xyz;
    tempNorm.y = mod(tempNorm.y, 1.0);
    myVertexNormal = tempNorm;
    myVertexPos = (modelViewMatrix * pos2).xyz;
    gl_Position = projectionMatrix * modelViewMatrix * pos2;
}