#version 330 core

layout (location = 0) in vec2 u_position;
layout (location = 1) in vec2 tex_coords;

out vec2 frag_tex_coords;

void main() {
    gl_Position = vec4(u_position, 0.0f, 1.0f);
    frag_tex_coords = tex_coords;
}
