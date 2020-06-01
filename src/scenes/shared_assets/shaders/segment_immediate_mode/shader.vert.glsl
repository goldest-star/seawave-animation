//Draw segment between points p1 p2 passed as uniform
#version 330 core


layout (location = 0) in vec4 u; //expect value in ([0,1],0,0)

// view transform
uniform mat4 view;
// perspective matrix
uniform mat4 perspective;

// Extremities of the segment
uniform vec3 p1 = vec3(0.0, 0.0, 0.0);
uniform vec3 p2 = vec3(1.0, 0.0, 0.0);


void main()
{
    vec4 position_transformed = (1.0-u.x)*vec4(p1,1.0) + u.x*vec4(p2,1.0);
    gl_Position = perspective * view * position_transformed;
}
