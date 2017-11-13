#include "include/game_object.h"

GameObject::GameObject() {
    id_ = -1;
    tangible_ = false;
    texture_ = 'U';
    position_ = vec3(0,0,0);
    scale_ = vec3(1, 1, 1);
    rotate_ = vec3(0,0,0);
}

GameObject::GameObject(int id, vec3 p, bool tangible, char texture) {
    id_ = id;
    tangible_ = tangible;
    texture_ = texture;
    position_ = p;
    scale_ = vec3(1, 1, 1);
    rotate_ = vec3(0,0,0);
}

mat4 GameObject::getModelMatrix() {
    mat4 model(1.0f);
    model = translate(model, position_);
    model = rotate(model, rotate_.y, vec3(0, 1, 0));
    model = rotate(model, rotate_.x, vec3(1, 0, 0));
    model = rotate(model, rotate_.z, vec3(0, 0, 1));
    model = scale(model, scale_);
    return model;
}

void GameObject::SendModel(GLuint prgm) {
    GLint uniModel = glGetUniformLocation(prgm, "model");
    mat4 model = getModelMatrix();
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, &model[0][0]);
}

void GameObject::SendMaterial(GLuint prgm) {
    GLint loc;
    vec3 v;
    loc  = glGetUniformLocation(prgm, "ka");
    v = ka_;
    glUniform3f(loc, v.x, v.y, v.z);
    loc  = glGetUniformLocation(prgm, "kd");
    v = kd_;
    glUniform3f(loc, v.x, v.y, v.z);
    loc  = glGetUniformLocation(prgm, "ks");
    v = ks_;
    glUniform3f(loc, v.x, v.y, v.z);
}
