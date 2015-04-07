#version 330 core

uniform mat4x4 CTM;
in vec3 House;

void main() {
    gl_Position = CTM * vec4(House, 1.0f);
}