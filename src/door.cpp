#include "include/door.h"

Door::Door() {
    open_ = false;
    id_ = (int) GAME_IDS::DOOR;
    door_id_ = 0;
    texture_ = 'A' + door_id_;
}

Door::Door(vec3 pos, int dID, bool open, char t) : GameObject((int) GAME_IDS::DOOR, pos, false, t) {
    open_ = open;
    door_id_ = dID;
}

void Door::InsertKey(Key& k) {
    if (k.getDoorID() == door_id_)
        open_ = true;
}
