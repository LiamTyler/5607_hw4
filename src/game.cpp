#include "include/game.h"
#include "include/wall.h"
#include "include/door.h"
#include "include/player.h"
#include "include/key.h"
#include "include/shape_vertices.h"
#include <fstream>
#include <cstdlib>

using namespace std;

Game::Game() {
    map_ = nullptr;
    start_pos_ = vec3(0,0,0);
    end_pos_ = vec3(0,0,0);
    aspect_ = 1;
    scale_ = vec3(10, 10, 10);
}

Game::Game(string fname) {
    map_ = nullptr;
    start_pos_ = vec3(0,0,0);
    end_pos_ = vec3(0,0,0);
    aspect_ = 1;
    scale_ = vec3(10, 10, 10);
    camera_pos_ = vec4(0.f, 1.0f, 50.0f, 1);
    camera_lookAt_ = vec4(0,0,-1, 0);
    camera_up_ = vec4(0, 1, 0, 0);
    // camera_rotation_ = vec4((float)M_PI/10, (float) -M_PI/10, 0.f, 0);
    camera_rotation_ = vec4(0.f, 0.f, 0.f, 0.f);
    camera_rot_mat_ = mat4(1.0f);
    speed_ = 20;
    Parse(fname);
}

void Game::Update(float dt) {
    float y = camera_pos_.y;
    camera_pos_ += speed_*dt*camera_rot_mat_*vec4(camera_vel_, 0);
    camera_pos_.y = y;
}

void Game::UpdateCameraAngle(float xrel, float yrel) {
    xrel *= -M_PI / 180.0 / 4;
    yrel *= -M_PI / 180.0 / 4;
    camera_rotation_ += vec4(yrel, xrel, 0.f, 0.f);
    mat4 r(1.0f);
    r = rotate(r, camera_rotation_.y, vec3(0, 1, 0));
    r = rotate(r, camera_rotation_.x, vec3(1, 0, 0));
    r = rotate(r, camera_rotation_.z, vec3(0, 0, 1));
    camera_rot_mat_ = r;
}

void Game::Init() {
    // Light
    dirLight_ = new DirectionalLight(
            vec3(-.5, -1, -.2),
            vec3(.3, .3, .3),
            vec3(.7, .7, .7),
            vec3(1.0, 1.0, 1.0));

    // Walls
    glGenVertexArrays(1, &cube_vao_);
    glBindVertexArray(cube_vao_);

    glGenBuffers(1, &cube_verts_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, cube_verts_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &cube_normals_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, cube_normals_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_normals), cube_normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Floor
    glGenVertexArrays(1, &floor_vao_);
    glBindVertexArray(floor_vao_);

    glGenBuffers(1, &floor_verts_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, floor_verts_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floor_vertices), floor_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &floor_normals_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, floor_normals_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floor_normals), floor_normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

Game::~Game() {
    for (int r = 0; r < height_; r++)
        for (int c = 0; c < width_; c++)
            if (map_[r*width_ + c] != nullptr)
                delete map_[r*width_ + c];
    delete map_;
}

int Game::getID(int r, int c) {
    if (map_[r*width_ + c])
        return map_[r*width_ + c]->getID();
    else
        return 0;
}

void Game::Draw(GLuint program) {
    GameObject* c = new GameObject();
    c->setMaterial(vec3(.4, .4, .4), vec3(1,0,0), vec3(1,1,1));
    c->setPosition(vec3(0, 5, 0));
    c->setScale(vec3(5, 5, 5));
    GameObject* f = new GameObject();
    f->setMaterial(vec3(.4, .4, .4), vec3(.7,.7,.7), vec3(1,1,1));
    f->setScale(vec3(500, 1, 500));


    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLint uniView  = glGetUniformLocation(program, "view");
    GLint uniProj  = glGetUniformLocation(program, "proj");

    mat4 view = lookAt(
            vec3(camera_pos_),
            vec3(camera_pos_ + camera_rot_mat_*camera_lookAt_),
            vec3(camera_rot_mat_*camera_up_));

    mat4 proj = perspective(radians(45.0f), aspect_, .1f, 100.0f);
    glUniformMatrix4fv(uniView, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, &proj[0][0]);

    GLint uniLight = glGetUniformLocation(program, "lightInEyeSpace");
    vec4 l = view * vec4(dirLight_->Dir(), 0);
    glUniform4f(uniLight, l[0], l[1], l[2], l[3]);
    dirLight_->Send(program);

    glBindVertexArray(cube_vao_);
    c->SendModel(program);
    c->SendMaterial(program);
    glDrawArrays(GL_TRIANGLES, 0, 12*3);


    glBindVertexArray(floor_vao_);
    f->SendModel(program);
    f->SendMaterial(program);
    glDrawArrays(GL_TRIANGLES, 0, 2*3);


    delete c;
    delete f;

}

bool Game::Parse(string fname) {
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
            vec3 p = vec3(c*scale_.x, scale_.y / 2, r*scale_.z);
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
