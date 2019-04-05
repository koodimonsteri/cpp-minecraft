#version 330

in vec3 textureCoord;

uniform sampler2DArray sb_sampler;
uniform vec3           ambientLight;

out vec4 fragColor;

void main()
{
    fragColor = texture(sb_sampler, textureCoord) * vec4(ambientLight, 1);
}