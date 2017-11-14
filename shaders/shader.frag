#version 330 core

uniform vec4 lightInEyeSpace;
uniform sampler2D tex;

uniform vec3 lights[80];
uniform int num_lights;

uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;

uniform mat4 view;

uniform float fade;
uniform bool textured;

const float s = 100;

out vec4 color;
in vec2 texCoord;
in vec3 vertexInEyeSpace;
in vec3 normalInEyeSpace;

/*
const vec3 lp = vec3(15, 1, 20);
const vec3 la = vec3(.3, .3, .3);
const vec3 ld = 10*vec3(10, 10, 10);
const vec3 ls = vec3(1, 1, 1);
*/

void main() {
    // float f = fade / 2;
    vec3 n = normalize(normalInEyeSpace);
    vec3 e = normalize(-vertexInEyeSpace);
    vec3 finalColor = vec3(0.0, 0.0, 0.0);

    /*
    vec3 light = vec3(view*vec4(lp, 1));

    vec3 l = normalize(light - vertexInEyeSpace);
    vec3 h = normalize(l + e);
    float diffuse = max(dot(l, n), 0.0);
    float specular = pow(max(dot(n, h), 0.0), s);

    float F = 1.0 / length(vertexInEyeSpace - light);
    if (textured) {
        finalColor += ka*la;
        finalColor += kd*ld * diffuse * vec3(texture(tex, texCoord));
        finalColor += ks*ls * specular;
    } else {
        finalColor += ka*la;
        finalColor += kd*ld * diffuse;
        finalColor += ks*ls * specular;
    }
    finalColor *= F;
    */
    for (int i = 0; i < num_lights; i++) {
        vec3 tmpColor = vec3(0,0,0);
        vec3 pl = lights[4*i + 0];
        vec3 lIa = lights[4*i + 1];
        vec3 lId = lights[4*i + 2];
        vec3 lIs = lights[4*i + 3];
        vec3 light = vec3(view*vec4(pl, 1));

        vec3 l = normalize(light - vertexInEyeSpace);
        vec3 h = normalize(l + e);
        float diffuse = max(dot(l, n), 0.0);
        float specular = pow(max(dot(n, h), 0.0), s);

        float F = 1.0 / pow(length(vertexInEyeSpace - light), 2);
        if (textured) {
            tmpColor += ka*lIa;
            tmpColor += kd*lId * diffuse * vec3(texture(tex, texCoord));
            tmpColor += ks*lIs * specular;
        } else {
            tmpColor += ka*lIa;
            tmpColor += kd*lId * diffuse;
            tmpColor += ks*lIs * specular;
        }
        finalColor += tmpColor*F;
    }
    /*
    vec3 l = normalize(-lightInEyeSpace.xyz);
    vec3 h = normalize(l + e);
    float diffuse = max(dot(l, n), 0.0);
    float specular = pow(max(dot(n, h), 0.0), s);

    if (textured) {
        finalColor += ka*Ia;
        finalColor += kd*Id * diffuse * vec3(texture(tex, texCoord));
        finalColor += ks*Is * specular;
    } else {
        finalColor += ka*Ia;
        finalColor += kd*Id * diffuse;
        finalColor += ks*Is * specular;
    }
    */

    // color.rgb = (1 - f)*finalColor + f*vec3(1,1,1);
    color.rgb = finalColor;
    color.a = 1.0;
}
