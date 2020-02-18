#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;

uniform vec2 u_resolution;
uniform float u_time;
uniform sampler2D u_texture;

void main() {
    finalColor = vec4(0.0, 0.0, 1.0, 1.0);
}
