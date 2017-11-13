#include "include/key.h"

Key::Key() {
    door_id_ = 0;
    id_ = (int) GAME_IDS::KEY;
    texture_ = 'a' + door_id_;
}

Key::Key(vec3 pos, int door_id, char t) : GameObject((int) GAME_IDS::KEY, pos, true, t) {
    door_id_ = door_id;
}
