#include "include/key.h"

Key::Key() {
    door_id_ = 0;
    id_ = (int) GAME_IDS::KEY;
    texture_ = 'a' + door_id_;
}

Key::Key(vec3 pos, int door_id, char t) : GameObject((int) GAME_IDS::KEY, pos, true, t) {
    door_id_ = door_id;
}

mat4 Key::getModelMatrix() {
    mat4 model(1.0f);
    model = translate(model, position_);
    model = rotate(model, rotate_.z, vec3(0, 0, 1));
    model = rotate(model, rotate_.y, vec3(0, 1, 0));
    // model = rotate(model, rotate_.x, vec3(1, 0, 0));
    model = scale(model, scale_);
    return model;
}
