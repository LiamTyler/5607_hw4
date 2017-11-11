#include "include/game_object.h"

GameObject::GameObject() {
    id_ = -1;
    pos_ = ivec2(0, 0);
    tangible_ = false;
    texture_ = 'U';
}

GameObject::GameObject(int id, ivec2 p, bool tangible, char texture) {
    id_ = id;
    pos_ = p;
    tangible_ = tangible;
    texture_ = texture;
}
