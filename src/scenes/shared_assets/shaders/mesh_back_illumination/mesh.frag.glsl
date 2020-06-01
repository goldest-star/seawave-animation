#version 330 core

in struct fragment_data
{
    vec4 position;
    vec4 normal;
    vec4 color;
    vec2 texture_uv;
} fragment;

uniform sampler2D texture_sampler;

out vec4 FragColor;

uniform vec3 camera_position;
uniform vec3 color     = vec3(1.0, 1.0, 1.0);
uniform float color_alpha = 1.0;
uniform float ambiant  = 0.2;
uniform float diffuse  = 0.8;
uniform float specular = 0.5;
uniform int specular_exponent = 128;

vec3 light = camera_position+vec3(+1.0, +1.0, 0.0);

void main()
{
    vec3 n = normalize(fragment.normal.xyz);
    vec3 u = normalize(light-fragment.position.xyz);
    vec3 r = reflect(u,n);
    vec3 t = normalize(fragment.position.xyz-camera_position);


    float diffuse_value  = diffuse * abs( dot(u,n) );
    float specular_value = specular * pow( abs( dot(r,t) ), specular_exponent);

    vec3 white = vec3(1.0);
    vec4 color_texture = texture(texture_sampler, fragment.texture_uv);
    vec3 c = (ambiant+diffuse_value)*color.rgb*fragment.color.rgb*color_texture.rgb + specular_value*white;

    FragColor = vec4(c, color_texture.a*fragment.color.a*color_alpha);
}
