#version 330 core

uniform vec4 lightInEyeSpace;

uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;

const float s = 50;

out vec4 color;
in vec3 vertexInEyeSpace;
in vec3 normalInEyeSpace;

void main() {
    vec3 n = normalize(normalInEyeSpace);
    vec3 l = normalize(-lightInEyeSpace.xyz);
    vec3 e = normalize(-vertexInEyeSpace);
    vec3 h = normalize(l + e);

    float diffuse = max(dot(l, n), 0.0);

    vec3 finalColor = vec3(0.0, 0.0, 0.0);
    finalColor += ka*Ia;
    finalColor += kd*Id * diffuse;

    color.rgb = finalColor;
    color.a = 1.0;
}
