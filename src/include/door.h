#ifndef SRC_INCLUDE_DOOR_H_
#define SRC_INCLUDE_DOOR_H_

#include <iostream>
#include <string>
#include "include/game_object.h"
#include "include/key.h"

class Door : public GameObject {
    public:
        Door();
        Door(ivec2 pos, int door_id, bool open, char t);

        void InsertKey(Key& k);
        bool getOpenStatus() { return open_; }

    protected:
        bool open_;
        int door_id_;
};

#endif  // SRC_INCLUDE_DOOR_H_
