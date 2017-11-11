#include "include/player.h"

Player::Player() {
    id_ = (int)GAME_IDS::PLAYER;
    health_ = 10;
    texture_ = 'P';
}

Player::Player(ivec2 pos, int health, char t) : GameObject((int) GAME_IDS::PLAYER, pos, false, t) {
    health_ = health;
}
