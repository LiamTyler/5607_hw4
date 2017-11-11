#ifndef SRC_INCLUDE_PLAYER_H_
#define SRC_INCLUDE_PLAYER_H_

#include <iostream>
#include <string>
#include "include/game_object.h"

using namespace std;

class Player : public GameObject {
    public:
        Player();
        Player(ivec2 pos, int health, char texture);

        int getHealth() { return health_; }
        void Damage(int d) { health_ -= d; }
        bool Alive() { return health_ > 0; }

    protected:
        int health_;
};

#endif  // SRC_INCLUDE_PLAYER_H_
