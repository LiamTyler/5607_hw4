#ifndef SRC_INCLUDE_MAP_H_
#define SRC_INCLUDE_MAP_H_

#include <iostream>
#include <string>
#include "include/utils.h"
#include "include/game_object.h"
#include "include/player.h"


using namespace std;

class Map {
    public:
        Map();
        Map(string fname);
        ~Map();
        bool Parse(string fname);
        void Draw();
        
        int getID(int r, int c);
        int getWidth() { return width_; }
        int getHeight() { return height_; }
        GameObject* getObject(int r, int c) { return map_[r*width_ + c]; }
        Player* getPlayer() { return player_; }

        ivec2 getStartPos() { return start_pos_; }
        ivec2 getEndPos() { return end_pos_; }

    private:
        int width_;
        int height_;
        Player* player_;
        GameObject** map_;
        ivec2 start_pos_;
        ivec2 end_pos_;
};

#endif  // SRC_INCLUDE_MAP_H_
