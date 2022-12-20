#version 450 core 
layout(location = 0) out vec4 FragmentColor;

in VS_OUT {
    vec2 TexCoords;
    vec3 FragmentPosition;
    vec3 TangentLightPosition;
    vec3 TangentViewPosition;
    vec3 TangentFragmentPosition;
} fs_in;

in vec3 Normal;

uniform sampler2D DiffuseTexture;
uniform sampler2D BumpTexture;
uniform vec2 BumpTextureResolution;

uniform vec3 LightPosition;
uniform vec3 ViewPosition;

vec3 computeNormalFromBumpTexture() {
    float du = 1.f / BumpTextureResolution.x;
    float dv = 1.f / BumpTextureResolution.y;
    
    float u_left = texture(BumpTexture, vec2(fs_in.TexCoords.x - du, fs_in.TexCoords.y)).r;
    float u_right = texture(BumpTexture, vec2(fs_in.TexCoords.x + du, fs_in.TexCoords.y)).r;
    float v_left = texture(BumpTexture, vec2(fs_in.TexCoords.x, fs_in.TexCoords.y - dv)).r;
    float v_right = texture(BumpTexture, vec2(fs_in.TexCoords.x, fs_in.TexCoords.y + dv)).r;

    float delta_u = (u_right - u_left) / 2.f;
    float delta_v = (v_right - v_left) / 2.f;

    return normalize(vec3(Normal.x + delta_u, Normal.y, Normal.z + delta_v));
}

void main() {
    vec3 normal = computeNormalFromBumpTexture();
    //normal = normalize(normal * 2.f - 1.f);
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
}