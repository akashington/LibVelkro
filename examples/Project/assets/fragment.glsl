#version 460 core

out vec4 FragColour;

in vec2 UVCoordinates;

uniform sampler2D u_Texture;

void main()
{
    FragColour = texture(u_Texture, UVCoordinates);
} 