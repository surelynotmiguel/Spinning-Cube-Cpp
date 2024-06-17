#version 450 core
    
precision mediump float;

layout (location = 0) in vec2 textureCoords;
out vec4 fColor;
uniform sampler2D uSampler;

void main() {
    fColor = texture(uSampler, textureCoords);
}
