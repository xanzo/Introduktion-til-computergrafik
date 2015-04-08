#version 330 core

in mat4x4 CTM;
in vec3 HousePos;
out vec4 gl_position;

void main() {
    gl_Position = CTM * vec4(HousePos, 1.0f);
}