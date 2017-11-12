#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;


uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 vertexInEyeSpace;
out vec3 normalInEyeSpace;

void main() {
    vertexInEyeSpace = (view * model * vec4(pos, 1)).xyz;
    normalInEyeSpace = normalize((transpose(inverse(view * model)) * vec4(normal, 0.0)).xyz);

    gl_Position = proj * view * model * vec4(pos, 1);
}
