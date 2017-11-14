#version 330 core

in vec3 pos;
in vec3 normal;
in vec2 inTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 vertexInEyeSpace;
out vec3 normalInEyeSpace;
out vec2 texCoord;

void main() {
    vertexInEyeSpace = (view * model * vec4(pos, 1)).xyz;
    normalInEyeSpace = normalize((transpose(inverse(view * model)) * vec4(normal, 0.0)).xyz);
    texCoord = inTexCoord;

    gl_Position = proj * view * model * vec4(pos, 1);
}
