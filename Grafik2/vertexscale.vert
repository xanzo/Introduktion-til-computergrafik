#version 330 core

uniform float Scale;
in vec3 VertexPosition;

void main() {
    gl_Position = vec4(Scale * VertexPosition.xy, VertexPosition.z, 1.0);
}
