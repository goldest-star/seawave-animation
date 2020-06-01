#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;

out struct vertex_data
{
    vec4 position;
    vec4 normal;
} vertex;

// model transformation
uniform vec3 translation = vec3(0.0, 0.0, 0.0);                      // user defined translation
uniform mat3 rotation = mat3(1.0,0.0,0.0, 0.0,1.0,0.0, 0.0,0.0,1.0); // user defined rotation
uniform float scaling = 1.0;                                         // user defined scaling
uniform vec3 scaling_axis = vec3(1.0,1.0,1.0);                       // user defined scaling


void main()
{
    // scaling matrix
    mat4 S = mat4(scaling*scaling_axis.x,0.0,0.0,0.0, 0.0,scaling*scaling_axis.y,0.0,0.0, 0.0,0.0,scaling*scaling_axis.z,0.0, 0.0,0.0,0.0,1.0);
    // 4x4 rotation matrix
    mat4 R = mat4(rotation);
    // 4D translation
    vec4 T = vec4(translation,0.0);

    vertex.position = R*S*position+T;
    vertex.normal = R*normal;
    gl_Position = vertex.position;
}
