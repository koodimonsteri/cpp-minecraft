#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nor;
layout (location = 2) in vec2 tex;

uniform mat4 viewProjectionMatrix;
uniform mat4 modelMatrix;

out vec4 fragColor;

void main() {

    fragColor = vec4(vec3(pos.y/16.0), 1.0);
    gl_Position = viewProjectionMatrix*modelMatrix*vec4(pos, 1.0);
} 