#include "include/wall.h"

Wall::Wall() {
    texture_ = 'W';
}

Wall::Wall(vec3 pos, char t) : GameObject((int) GAME_IDS::WALL, pos, false, t) {}
