#ifndef SRC_INCLUDE_WALL_H_
#define SRC_INCLUDE_WALL_H_

#include <iostream>
#include <string>
#include "include/game_object.h"

class Wall : public GameObject {
    public:
        Wall();
        Wall(ivec2, char texture);
};

#endif  // SRC_INCLUDE_WALL_H_
