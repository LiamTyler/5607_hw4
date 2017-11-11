#include "include/map.h"
#include "include/wall.h"
#include "include/door.h"
#include "include/player.h"
#include "include/key.h"
#include <fstream>
#include <cstdlib>

using namespace std;

Map::Map() {
    map_ = nullptr;
    start_pos_ = ivec2(0,0);
    end_pos_ = ivec2(0,0);
}

Map::Map(string fname) {
    map_ = nullptr;
    start_pos_ = ivec2(0,0);
    end_pos_ = ivec2(0,0);
    Parse(fname);
}

Map::~Map() {
    for (int r = 0; r < height_; r++)
        for (int c = 0; c < width_; c++)
            if (map_[r*width_ + c] != nullptr)
                delete map_[r*width_ + c];
    delete map_;
}

int Map::getID(int r, int c) {
    if (map_[r*width_ + c])
        return map_[r*width_ + c]->getID();
    else
        return 0;
}

bool Map::Parse(string fname) {
    ifstream in(fname);
    if (in.fail()) {
        cerr << "Failed to open file: " << fname << endl;
        return false;
    }

    in >> width_ >> height_;
    // surround the map with walls
    width_ += 2;
    height_ += 2;
    map_ = new GameObject*[width_*height_];
    for (int r = 0; r < height_; r++) {
        for (int c = 0; c < width_; c++) {
            ivec2 p = ivec2(r, c);
            if (c == 0 || c == width_ - 1 || r == 0 || r == height_ - 1) {
                map_[r*width_ + c] = new Wall(p, 'W');
            } else {
                GameObject* tmp = nullptr;
                char o;
                in >> o;
                switch(o) {
                    case 'W':
                        tmp = new Wall(p, 'W');
                        break;
                    case 'S':
                        start_pos_ = p;
                        tmp = new Player(p, 10, 'P');
                        break;
                    case 'G':
                        end_pos_ = p;
                        break;
                    case 'A':
                    case 'B':
                    case 'C':
                    case 'D':
                    case 'E':
                        tmp = new Door(p, o - 'A', false, o);
                        break;
                    case 'a':
                    case 'b':
                    case 'c':
                    case 'd':
                    case 'e':
                        tmp = new Key(p, o - 'A', o);
                        break;
                    default:
                        break;
                }
                map_[r*width_ + c] = tmp;
            }
        }
    }
    in.close();
    return true;
}
