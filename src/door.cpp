#include "include/door.h"

Door::Door() {
    open_ = false;
    id_ = (int) GAME_IDS::DOOR;
    door_id_ = 0;
    texture_ = 'A' + door_id_;
    open_time_ = 0;
}

Door::Door(vec3 pos, int dID, bool open, char t) : GameObject((int) GAME_IDS::DOOR, pos, false, t) {
    open_ = open;
    door_id_ = dID;
    open_time_ = 0;
}

void Door::Open() {
    open_ = true;
}

bool Door::Update(float dt) {
    const float time = 1.6;
    if (!open_)
        return false;
    open_time_ += dt;
    position_.y += scale_.y * dt / time * 3;

    if (open_time_ > time)
        return true;
    
    return false;
}
