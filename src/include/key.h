#ifndef SRC_INCLUDE_KEY_H_
#define SRC_INCLUDE_KEY_H_

#include <iostream>
#include <string>
#include "include/game_object.h"

class Key : public GameObject {
    public:
        Key();
        Key(vec3 pos, int door_id, char t);

        int getDoorID() { return door_id_; }

    protected:
        int door_id_;
};

#endif  // SRC_INCLUDE_KEY_H_
