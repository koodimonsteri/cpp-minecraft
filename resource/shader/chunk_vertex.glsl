#version 330

layout (location = 0) in uvec3 blockPos;
layout (location = 1) in int texId;
layout (location = 2) in int faceData;

// UNIFORM
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

// OUT
out mat2 aoTable;
out vec2 uv;
flat out int passTexId;
out vec3 myVertexPos;

// LOOK UP
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

void main() {
    createAoTable();
    passTexId = texId;
    uv = uvs[gl_VertexID % 4];
    vec4 pos = vec4(positions[4*(faceData & 7) + gl_VertexID % 4] + blockPos, 1);

    myVertexPos = (modelViewMatrix * pos).xyz;
    
    gl_Position = projectionMatrix * modelViewMatrix * pos;
}