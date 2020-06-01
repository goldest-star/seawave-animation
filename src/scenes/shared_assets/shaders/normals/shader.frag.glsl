#version 330 core

in struct fragment_data
{
    vec4 position;
} fragment;


out vec4 FragColor;

void main()
{
    FragColor = vec4(0.0, 0.0, 0.5, 1.0);
}
