#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec3 aBitangent;
layout(location = 4) in vec2 aUV;

out VS_OUT {
    vec2 TexCoords;
    vec3 FragmentPosition;
    vec3 TangentLightPosition;
    vec3 TangentViewPosition;
    vec3 TangentFragmentPosition;
} vs_out;

out vec3 Normal;

uniform vec3 LightPosition;
uniform vec3 ViewPosition;

uniform mat4 MVP;

void main() {
    vs_out.FragmentPosition = aPos;
    vs_out.TexCoords = aUV;
    Normal = aNormal;
    
    vec3 T = normalize(aTangent);
    vec3 N = normalize(aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    //B = normalize(aBitangent);

    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.TangentLightPosition = TBN * LightPosition;
    vs_out.TangentViewPosition = TBN * ViewPosition;
    vs_out.TangentFragmentPosition = TBN * vs_out.FragmentPosition;

    gl_Position = MVP * vec4(aPos, 1.f);
}