#version 330 core
in vec2 TexCoords;
in vec4 ParticleColor;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;

void main()
{
    color = ParticleColor * texture(image, TexCoords);
}