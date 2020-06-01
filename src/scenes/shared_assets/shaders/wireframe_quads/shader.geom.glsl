
#version 330

layout (triangles) in;
layout (line_strip, max_vertices=3) out;

in struct vertex_data
{
    vec4 position;
    vec4 normal;
} vertex[];



// view transform
uniform mat4 view;
// perspective matrix
uniform mat4 perspective;


void main(void)
{
    mat4 M = perspective * view;

    gl_Position = M*vertex[0].position;
    EmitVertex();
    gl_Position = M*vertex[1].position;
    EmitVertex();
    gl_Position = M*vertex[2].position;
    EmitVertex();

    EndPrimitive();
}
