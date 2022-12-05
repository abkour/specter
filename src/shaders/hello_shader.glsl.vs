#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;

out vec2 uv;

uniform mat4 MVP;

void main() {
    uv = aUV;
    gl_Position = MVP * vec4(aPos, 1.f);
}