#version 330 core

uniform vec4 lightInEyeSpace;

const vec4 Ia = vec4(0.3, 0.3, 0.3, 1);
const vec4 Id = vec4(0.7, 0.7, 0.7, 1);
const vec4 Is = vec4(1.0, 1.0, 1.0, 1);

const vec4 ka = vec4(.4, .4, .4, 1);
const vec4 kd = vec4(1.0, 0.0, 0.0, 1);
const vec4 ks = vec4(.6, .6, .6, 1);
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
    finalColor += (ka.xyz)*(Ia.xyz);
    finalColor += (kd.xyz)*(Id.xyz) * diffuse;

    color.rgb = finalColor;
    color.a = 1.0;
}
