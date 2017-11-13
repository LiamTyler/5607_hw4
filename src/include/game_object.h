#ifndef SRC_INCLUDE_GAME_OBJECT_H_
#define SRC_INCLUDE_GAME_OBJECT_H_

#include <iostream>
#include <string>
#include "include/utils.h"

using namespace std;

class GameObject {
    public:
        GameObject();
        GameObject(int id, vec3 pos, bool tangible, char texture);

        virtual void SendModel(GLuint prgm);
        void SendMaterial(GLuint prgm);
        mat4 getModelMatrix();

        void setMaterial(vec3 a, vec3 d, vec3 s) { ka_ = a; kd_ = d; ks_ = s; }
        void setPosition(vec3 p) { position_ = p; }
        void setScale(vec3 s) { scale_ = s; }
        void setRotate(vec3 r) { rotate_ = r; }
        int getID() { return id_; }
        bool getTangible() { return tangible_; }
        ivec2 getPosition() { return position_; }
        char getTexture() { return texture_; }
        void setID(int id) { id_ = id; }
        void setTangible(bool t) { tangible_ = t; }
        void setTexture(char c) { texture_ = c; }

    protected:
        int id_;
        bool tangible_;
        char texture_;
        vec3 position_;
        vec3 scale_;
        vec3 rotate_;
        vec3 ka_;
        vec3 kd_;
        vec3 ks_;
};

#endif  // SRC_INCLUDE_GAME_OBJECT_H_
