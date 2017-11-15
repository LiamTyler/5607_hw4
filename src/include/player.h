#ifndef SRC_INCLUDE_PLAYER_H_
#define SRC_INCLUDE_PLAYER_H_

#include <iostream>
#include <string>
#include "include/game_object.h"

using namespace std;

class Player : public GameObject {
    public:
        Player();
        Player(vec3 pos, int health, char texture);

        int getHealth() { return health_; }
        void Damage(int d) { health_ -= d; }
        bool Alive() { return health_ > 0; }
        float getRadius() { return radius_; }
        float getHeight() { return height_; }

    protected:
        int health_;
        float radius_;
        float height_;
};

#endif  // SRC_INCLUDE_PLAYER_H_
