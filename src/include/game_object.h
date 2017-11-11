#ifndef SRC_INCLUDE_GAME_OBJECT_H_
#define SRC_INCLUDE_GAME_OBJECT_H_

#include <iostream>
#include <string>
#include "include/utils.h"

using namespace std;

class GameObject {
    public:
        GameObject();
        GameObject(int id, ivec2, bool tangible, char texture);

        virtual void Draw() { cout << texture_; }

        int getID() { return id_; }
        bool getTangible() { return tangible_; }
        ivec2 getPosition() { return pos_; }
        char getTexture() { return texture_; }
        void setID(int id) { id_ = id; }
        void setTangible(bool t) { tangible_ = t; }
        void setPosition(ivec2 p) { pos_ = p; }
        void setTexture(char c) { texture_ = c; }

    protected:
        int id_;
        ivec2 pos_;
        bool tangible_;
        char texture_;
};

#endif  // SRC_INCLUDE_GAME_OBJECT_H_
