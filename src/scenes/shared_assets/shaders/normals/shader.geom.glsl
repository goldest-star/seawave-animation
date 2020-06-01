
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
    const float L = 0.03;

    gl_Position = M*vertex[0].position;
    EmitVertex();
    gl_Position = M*(vertex[0].position + vec4(L*vertex[0].normal.xyz,0));
    EmitVertex();
    gl_Position = M*vertex[0].position ;
    EmitVertex();


    EndPrimitive();
}
