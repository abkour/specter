#version 450 core 
layout(location = 0) out vec4 FragmentColor;

in vec2 uv;

uniform sampler2D diffuse_texture;

void main() {
    FragmentColor = texture(diffuse_texture, uv);
    //FragmentColor = vec4(1.f, 0.72f, 0.25f, 1.f);
}