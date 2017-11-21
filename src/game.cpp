#include "include/game.h"
#include "include/wall.h"
#include "include/door.h"
#include "include/player.h"
#include "include/key.h"
#include "include/shape_vertices.h"
#include "include/OBJ_Loader.h"
#include <fstream>
#include <cstdlib>

#define MAX_LIGHTS 20

using namespace std;

Game::Game(string fname) {
    // map variables
    map_ = nullptr;
    start_pos_ = vec3(0,0,0);
    end_pos_ = vec3(0,0,0);
    aspect_ = 1;
    scale_ = 20*vec3(1, 1, 1);
    speed_ = 10;
    hit_width_ = 2;
    grab_radius_ = 4;

    // camera
    camera_pos_ = vec4(0.f, 100.0f, 50.0f, 1);
    camera_lookAt_ = vec4(0,0,-1, 0);
    camera_up_ = vec4(0, 1, 0, 0);
    camera_rotation_ = vec4(0.f, 0.f, 0.f, 0.f);
    camera_rot_mat_ = mat4(1.0f);

    // Floor and ceiling
    floor_ = new GameObject();
    floor_->setMaterial(vec3(.4, .4, .4), vec3(.5,.5,.5), vec3(.1,.1,.1));
    floor_->setScale(vec3(500, 1, 500));
    floor_->setPosition(vec3(0,-0.01, 0));
    ceiling_ = new GameObject();
    ceiling_->setMaterial(vec3(.4, .4, .4), vec3(.7,.7,.7), vec3(.1,.1,.1));
    ceiling_->setScale(vec3(500, 1, 500));
    ceiling_->setPosition(vec3(0,scale_.y + 0.01, 0));

    // ending fade
    fading_ = false;
    fade_ = 0;
    grabbed_key_ = nullptr;

    // Lights
    num_point_lights_ = 0;
    point_lights_ = new Light[MAX_LIGHTS];

    Parse(fname);
}

Game::~Game() {
    for (int r = 0; r < height_; r++)
        for (int c = 0; c < width_; c++)
            if (map_[r*width_ + c] != nullptr)
                delete map_[r*width_ + c];
    delete map_;
    delete dirLight_;
    delete [] point_lights_;
}

void Game::Update(float dt) {
    float y = camera_pos_.y;
    vec4 npp = camera_pos_ + 5*speed_*dt*camera_rot_mat_*vec4(camera_vel_, 0);
    npp.y = y;
    vec3 np = vec3(npp);
    bool hit = false;
    for (int r = 0; r < height_ && !hit; r++) {
        for (int c = 0; c < width_ && !hit; c++) {
            GameObject* o = map_[r*width_ + c];
            if (o && o != grabbed_key_) {
                vec3 p = o->getPosition();
                vec3 s = o->getScale();
                float e = 0;
                if (grabbed_key_) 
                    e = 2*grab_radius_;
                float dx = abs(np.x - p.x);
                float dz = abs(np.z - p.z);
                if (dx < hit_width_ + s.x + e && dz < hit_width_ + s.z + e) {
                    hit = true;
                }
            }
        }
    }
    if (!hit)
        camera_pos_ = npp;

    if (grabbed_key_) {
        vec3 p = vec3(camera_pos_ + 2*grab_radius_*camera_rot_mat_*camera_lookAt_) - vec3(0, 1, 0);
        vec3 newR = vec3(camera_rotation_);
        newR.y += (float)M_PI / 2;
        mat4 rot(1.0f);
        grabbed_key_->setRotate(newR);
        grabbed_key_->setPosition(p);
    }
    for (int i = 0; i < doors_.size(); i++) {
        if (doors_[i]->Update(dt)) {
            // erase door
            for (int r = 0; r < height_; r++) {
                for (int c = 0; c < width_; c++) {
                    if (map_[r*width_ + c] == doors_[i]) {
                        map_[r*width_ + c] = nullptr;
                    }
                }
            }
            delete doors_[i];
            doors_.erase(doors_.begin() + i);
            i--;
        }
    }

    // if (length(vec3(camera_pos_) - end_pos_) <= 10) {
    //     fading_ = true;
    // }
    // if (fading_)
    //     fade_ += dt;
}

void Game::UpdateCameraAngle(float xrel, float yrel) {
    xrel *= -M_PI / 180.0 / 4;
    yrel *= -M_PI / 180.0 / 4;
    float p = M_PI / 4;
    float nx = camera_rotation_.x + yrel;
    float ny = camera_rotation_.y + xrel;
    nx = fmax(-p, fmin(p, nx));
    camera_rotation_ = vec4(nx, ny, camera_rotation_.z, camera_rotation_.w);
    // camera_rotation_ += vec4(yrel, xrel, 0.f, 0.f);
    mat4 r(1.0f);
    r = rotate(r, camera_rotation_.y, vec3(0, 1, 0));
    r = rotate(r, camera_rotation_.x, vec3(1, 0, 0));
    r = rotate(r, camera_rotation_.z, vec3(0, 0, 1));
    camera_rot_mat_ = r;
}

void Game::InteractKey() {
    if (grabbed_key_ == nullptr) {
        bool hit = false;
        for (int i = 0; i < keys_.size(); i++) {
            vec3 p = keys_[i]->getPosition();
            vec3 s = keys_[i]->getScale();
            s *= 5;
            float dx = abs(camera_pos_.x - p.x);
            float dz = abs(camera_pos_.z - p.z);
            if (dx < grab_radius_ + s.x && dz < grab_radius_ + s.z) {
                grabbed_key_ = keys_[i];
                hit = true;
            }
        }
        if (hit) {
            for (int r = 0; r < height_; r++) {
                for (int c = 0; c < width_; c++) {
                    if (map_[r*width_ + c] == grabbed_key_) {
                        map_[r*width_ + c] = nullptr;
                    }
                }
            }
        }
    } else {
        int open_door = -1;
        for (int i = 0; i < doors_.size(); i++) {
            Door* d = doors_[i];
            vec3 dp = d->getPosition();
            vec3 ds = d->getScale();
            vec3 kp = grabbed_key_->getPosition();
            vec3 ks = grabbed_key_->getScale();
            float l = length(dp - kp);
            if (l < grab_radius_ + ks.x + ds.x &&
                l < grab_radius_ + ks.z + ds.z) {
                if (grabbed_key_->getDoorID() == d->getDoorID()) {
                    d->Open();
                    open_door = i;
                }
            }
        }
        if (open_door != -1) {
            // erase key
            int gk;
            for (int i = 0; i < keys_.size(); ++i) {
                if (grabbed_key_ == keys_[i])
                    gk = i;
            }
            keys_.erase(keys_.begin() + gk);
            delete grabbed_key_;
            grabbed_key_ = nullptr;
        }
    }
}

void Game::Init(GLuint program) {
    // Light
    dirLight_ = new Light(
            vec3(-.5, -1, -.2),
            vec3(.3, .3, .3),
            vec3(.7, .7, .7),
            vec3(1.0, 1.0, 1.0));

    glUniform3fv(glGetUniformLocation(program, "lights"), 4*num_point_lights_, (GLfloat*)point_lights_);

    // num_point_lights_ = 1;
    glUniform1i(glGetUniformLocation(program, "num_lights"), num_point_lights_);

    // Walls
    glGenVertexArrays(1, &cube_vao_);
    glBindVertexArray(cube_vao_);

    glGenBuffers(1, &cube_verts_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, cube_verts_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
    GLint posAttrib = glGetAttribLocation(program, "pos");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &cube_normals_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, cube_normals_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_normals), cube_normals, GL_STATIC_DRAW);
    GLint normalAttrib = glGetAttribLocation(program, "normal");
    glEnableVertexAttribArray(normalAttrib);
    glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &cube_uv_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, cube_uv_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_UV), cube_UV, GL_STATIC_DRAW);
    GLint texAttrib = glGetAttribLocation(program, "inTexCoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Floor
    glGenVertexArrays(1, &floor_vao_);
    glBindVertexArray(floor_vao_);

    glGenBuffers(1, &floor_verts_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, floor_verts_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floor_vertices), floor_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &floor_normals_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, floor_normals_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floor_normals), floor_normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(normalAttrib);
    glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Ceiling
    glGenVertexArrays(1, &ceiling_vao_);
    glBindVertexArray(ceiling_vao_);

    glGenBuffers(1, &ceiling_verts_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, ceiling_verts_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ceiling_vertices), ceiling_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &ceiling_normals_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, ceiling_normals_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ceiling_normals), ceiling_normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(normalAttrib);
    glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Textures
    glActiveTexture(GL_TEXTURE0);
    wall_texture_ = LoadTexture("textures/wall.bmp");
    door_textures_.push_back(LoadTexture("textures/door_A.bmp"));
    door_textures_.push_back(LoadTexture("textures/door_B.bmp"));
    door_textures_.push_back(LoadTexture("textures/door_C.bmp"));
    door_textures_.push_back(LoadTexture("textures/door_D.bmp"));
    door_textures_.push_back(LoadTexture("textures/door_E.bmp"));

    // key objs
    objl::Loader Loader;
    bool loadout = Loader.LoadFile("key.obj");
    if (!loadout) {
        cout << "failed to open/load obj file" << endl;
        return;
    }
    objl::Mesh m = Loader.LoadedMeshes[0];
    vector<vec3> verts;
    vector<vec3> norms;
    for (int i = 0; i < m.Vertices.size(); i++) {
        float x,y,z;
        x = m.Vertices[i].Position.X;
        y = m.Vertices[i].Position.Y;
        z = m.Vertices[i].Position.Z;
        verts.push_back(vec3(x,y,z));
        x = m.Vertices[i].Normal.X;
        y = m.Vertices[i].Normal.Y;
        z = m.Vertices[i].Normal.Z;
        norms.push_back(vec3(x,y,z));
    }
    obj_tris_ = m.Indices.size() / 3;
    vector<unsigned int> indices;
    for (int i = 0; i < m.Indices.size(); i++) {
        indices.push_back(m.Indices[i]);
    }

    glGenVertexArrays(1, &obj_vao_);
    glBindVertexArray(obj_vao_);

    glGenBuffers(1, &obj_verts_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, obj_verts_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*m.Vertices.size(), &verts[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &obj_normals_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, obj_normals_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*m.Vertices.size(), &norms[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(normalAttrib);
    glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &obj_ibo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj_ibo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*m.Indices.size(), &indices[0], GL_STATIC_DRAW);

}

int Game::getID(int r, int c) {
    if (map_[r*width_ + c])
        return map_[r*width_ + c]->getID();
    else
        return 0;
}

void Game::Draw(GLuint program) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLint uniView  = glGetUniformLocation(program, "view");
    GLint uniProj  = glGetUniformLocation(program, "proj");

    mat4 view = lookAt(
            vec3(camera_pos_),
            vec3(camera_pos_ + camera_rot_mat_*camera_lookAt_),
            vec3(camera_rot_mat_*camera_up_));

    mat4 proj = perspective(radians(45.0f), aspect_, .1f, 400.0f);
    glUniformMatrix4fv(uniView, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, &proj[0][0]);

    GLint uniLight = glGetUniformLocation(program, "lightInEyeSpace");
    vec4 l = view * vec4(dirLight_->Dir(), 0);
    glUniform4f(uniLight, l[0], l[1], l[2], l[3]);
    dirLight_->Send(program);

    GLint fade = glGetUniformLocation(program, "fade");
    glUniform1f(fade, fade_);

    GLint textured = glGetUniformLocation(program, "textured");
    glUniform1f(textured, true);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, wall_texture_);
    glUniform1i(glGetUniformLocation(program, "tex"), 0);


    glBindVertexArray(cube_vao_);
    for (int r = 0; r < height_; r++) {
        for (int c = 0; c < width_; c++) {
            GameObject* o = map_[r*width_ + c];
            int id = getID(r, c);
            if (id == (int)GAME_IDS::WALL) {
                o->SendModel(program);
                o->SendMaterial(program);
                glDrawArrays(GL_TRIANGLES, 0, 12*3);
            }
        }
    }

    /*
    glUniform1f(textured, false);
    for (int i = 0; i < keys_.size(); i++) {
        keys_[i]->SendModel(program);
        keys_[i]->SendMaterial(program);
        glDrawArrays(GL_TRIANGLES, 0, 12*3);
    }
    */

    glUniform1f(textured, true);
    for (int i = 0; i < doors_.size(); i++) {
        Door* d = doors_[i];
        d->SendModel(program);
        d->SendMaterial(program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, door_textures_[d->getDoorID()]);
        glUniform1i(glGetUniformLocation(program, "tex"), 0);
        glDrawArrays(GL_TRIANGLES, 0, 12*3);
    }

    glBindVertexArray(obj_vao_);
    glUniform1f(textured, false);
    for (int i = 0; i < keys_.size(); i++) {
        keys_[i]->SendModel(program);
        keys_[i]->SendMaterial(program);
        glDrawElements(GL_TRIANGLES, obj_tris_*3, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(floor_vao_);
    glUniform1f(textured, false);
    floor_->SendModel(program);
    floor_->SendMaterial(program);
    glDrawArrays(GL_TRIANGLES, 0, 2*3);

    glBindVertexArray(ceiling_vao_);
    glUniform1f(textured, false);
    ceiling_->SendModel(program);
    ceiling_->SendMaterial(program);
    glDrawArrays(GL_TRIANGLES, 0, 2*3);
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
    vec3 wa = vec3(.4, .4, .4);
    vec3 wd = vec3(1, 1, 1);
    vec3 ws = vec3(1, 1, 1);
    vec3 la = 5*vec3(.3, .3, .3);
    vec3 ld = 100*vec3(.7, .7, .7);
    vec3 ls(0.8, 0.8, 0.8);
    for (int r = 0; r < height_; r++) {
        for (int c = 0; c < width_; c++) {
            vec3 p = vec3(c*scale_.x, scale_.y / 2, r*scale_.z);
            if (c == 0 || c == width_ - 1 || r == 0 || r == height_ - 1) {
                map_[r*width_ + c] = new Wall(p, 'W');
                map_[r*width_ + c]->setScale(.5 * scale_);
                map_[r*width_ + c]->setMaterial(wa, wd, ws);
            } else {
                GameObject* tmp = nullptr;
                char o;
                in >> o;
                switch(o) {
                    case 'W':
                        tmp = new Wall(p, 'W');
                        tmp->setScale(.5*scale_);
                        tmp->setMaterial(wa, wd, ws);
                        break;
                    case 'S':
                        start_pos_ = p;
                        // tmp = new Player(p, 10, 'P');
                        camera_pos_ = vec4(p, 1);
                        break;
                    case 'G':
                        end_pos_ = p;
                        break;
                    case 'L':
                        if (num_point_lights_ == MAX_LIGHTS)
                            break;
                        // cout << p << endl;
                        point_lights_[num_point_lights_++] = Light(p, la, ld, ls);
                        break;
                    case 'A':
                    case 'B':
                    case 'C':
                    case 'D':
                    case 'E':
                        {
                        Door* d = new Door(p, o - 'A', false, o);
                        d->setScale(.5*scale_);
                        d->setMaterial(wa, wd, ws);
                        doors_.push_back(d);
                        tmp = d;
                        break;
                        }
                    case 'a':
                    case 'b':
                    case 'c':
                    case 'd':
                    case 'e':
                    {
                        p.y -= 4;
                        Key *k= new Key(p, o - 'a', o);
                        int id = o - 'a';
                        vec3 ka = vec3(.3, .3, .3);
                        vec3 kd;
                        vec3 ks = vec3(1, 1, 1);
                        if (id == 0)
                            kd = vec3(0, 1, 1);
                        else if (id == 1)
                            kd = vec3(1, 0, 1);
                        else if (id == 2)
                            kd = vec3(1, 0, 0);
                        else if (id == 3)
                            kd = vec3(0, 1, 0);
                        else if (id == 4)
                            kd = vec3(1, 1, 0);
                        k->setScale(vec3(1, 1, 1));
                        k->setRotate(vec3(0, (float)M_PI / 2, 0));
                        k->setMaterial(ka, kd, ks);
                        keys_.push_back(k);
                        tmp = k;
                        break;
                    }
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
