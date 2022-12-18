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
    vec3 ambient = 0.1f * color;

    vec3 lightDirection = normalize(fs_in.TangentLightPosition - fs_in.TangentFragmentPosition);
    float diff = max(dot(lightDirection, normal), 0.f);
    vec3 diffuse = diff * color;

    vec3 viewDirection = normalize(fs_in.TangentViewPosition - fs_in.TangentFragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    float spec = pow(max(dot(normal, halfwayDirection), 0.f), 32.f);

    vec3 specular = vec3(0.2f) * spec;
    FragmentColor = vec4(ambient + diffuse + specular, 1.f);
    FragmentColor = vec4(color, 1.f);
}