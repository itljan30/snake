#version 330 core

in vec2 frag_tex_coords;

uniform sampler2D tex_sample;

out vec4 frag_color;

void main() {
    frag_color = texture(tex_sample, frag_tex_coords);
}
