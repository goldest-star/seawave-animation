#version 330 core

in struct fragment_data
{
    vec4 position;
} fragment;


out vec4 FragColor;
uniform vec4 wireframe_color = vec4(0.0, 0.0, 0.5, 1.0);

void main()
{
    FragColor = wireframe_color;
}
