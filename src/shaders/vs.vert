#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 textCoords;
out vec2 textureCoords;
uniform vec3 theta;

void main() {
    textureCoords = textCoords;

    vec3 angles = radians(theta);
    vec3 c = cos(angles);
    vec3 s = sin(angles);

    mat4 rx = mat4( 
        1.0, 0.0, 0.0, 0.0,
        0.0, c.x, s.x, 0.0,
        0.0, -s.x, c.x, 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    mat4 rz = mat4(
        c.z, -s.z, 0.0, 0.0,
        s.z, c.z, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    mat4 ry = mat4( 
        c.y, 0.0, s.y, 0.0,
        0.0, 1.0, 0.0, 0.0,
        -s.y, 0.0, c.y, 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    gl_Position = rz * ry * rx * vec4(aPosition, 1.0);
}
