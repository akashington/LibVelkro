#version 460 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 UV;

out vec2 UVCoordinates;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    gl_Position = u_Projection * u_View * mat4(1.0) * vec4(Position.xyz, 1.0);

    UVCoordinates = UV;
}