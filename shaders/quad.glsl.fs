#version 460 core
layout(location = 0) out vec4 FragmentColor;

in vec2 texCoord;

uniform sampler2D texture0;

void main() {
    FragmentColor = texture(texture0, texCoord);
    //FragmentColor = vec4(0.f, 0.f, 1.f, 1.f);
}