#version 450 core 
layout(location = 0) out vec4 FragmentColor;

in VS_OUT {
    vec2 TexCoords;
    vec3 FragmentPosition;
    vec3 TangentLightPosition;
    vec3 TangentViewPosition;
    vec3 TangentFragmentPosition;
} fs_in;

uniform sampler2D DiffuseTexture;
uniform sampler2D NormalTexture;

uniform vec3 LightPosition;
uniform vec3 ViewPosition;

void main() {
    vec3 normal = texture(NormalTexture, fs_in.TexCoords).rgb;
    normal = normalize(normal * 2.f - 1.f);

    vec3 color = texture(DiffuseTexture, fs_in.TexCoords).rgb;
    FragmentColor = vec4(color, 1.f);
}