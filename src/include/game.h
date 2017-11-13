#ifndef SRC_INCLUDE_GAME_H_
#define SRC_INCLUDE_GAME_H_

#include <iostream>
#include <string>
#include "include/utils.h"
#include "include/game_object.h"
#include "include/player.h"
#include "include/lights.h"

using namespace std;

class Game {
    public:
        Game();
        Game(string fname);
        ~Game();
        void Init();
        bool Parse(string fname);
        void Draw(GLuint program);
        
        void setAspectRatio(float a) { aspect_ = a; }
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
        float aspect_;
        vec3 scale_;
        Player* player_;
        GameObject** map_;
        vec3 start_pos_;
        vec3 end_pos_;
        DirectionalLight* dirLight_;

        GLuint cube_vao_;
        GLuint cube_verts_vbo_;
        GLuint cube_normals_vbo_;
        GLuint floor_vao_;
        GLuint floor_verts_vbo_;
        GLuint floor_normals_vbo_;
};

#endif  // SRC_INCLUDE_GAME_H_
