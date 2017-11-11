#ifndef SRC_INCLUDE_UTILS_H_
#define SRC_INCLUDE_UTILS_H_

#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace glm;

enum class GAME_IDS : int {
    EMPTY = 0,
    WALL,
    PLAYER,
    DOOR,
    KEY,
};

#endif  // SRC_INCLUDE_UTILS_H_
