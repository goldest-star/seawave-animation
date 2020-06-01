#version 330 core

layout (location = 0) in vec4 position;



// model transformation
uniform vec3 translation = vec3(0.0, 0.0, 0.0);                      // user defined translation
uniform mat3 rotation = mat3(1.0,0.0,0.0, 0.0,1.0,0.0, 0.0,0.0,1.0); // user defined rotation
uniform float scaling = 1.0;                                         // user defined scaling

// view transform
uniform mat4 view;
// perspective matrix
uniform mat4 perspective;



void main()
{
    // scaling matrix
    mat4 S = mat4(scaling,0.0,0.0,0.0, 0.0,scaling,0.0,0.0, 0.0,0.0,scaling,0.0, 0.0,0.0,0.0,1.0);
    // 4x4 rotation matrix
    mat4 R = mat4(rotation);
    // 4D translation
    vec4 T = vec4(translation,0.0);

    vec4 position_transformed = R*S*position + T;
    gl_Position = perspective * view * position_transformed;
}
