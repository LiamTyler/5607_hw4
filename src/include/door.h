#ifndef SRC_INCLUDE_DOOR_H_
#define SRC_INCLUDE_DOOR_H_

#include <iostream>
#include <string>
#include "include/game_object.h"
#include "include/key.h"

class Door : public GameObject {
    public:
        Door();
        Door(vec3 pos, int door_id, bool open, char t);

        void Open();
        bool Update(float dt);
        bool getOpenStatus() { return open_; }
        int getDoorID() { return door_id_; }

    protected:
        bool open_;
        float open_time_;
        int door_id_;
};

#endif  // SRC_INCLUDE_DOOR_H_
