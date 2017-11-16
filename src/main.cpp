#include <iostream>
#include <vector>
#include "include/game.h"
#include "include/shape_vertices.h"

using namespace std;

string MAP = "/home/liam/Documents/School/5607/hw4/maps/base.txt";
string VERTEX_SHADER = "/home/liam/Documents/School/5607/hw4/shaders/shader.vert";
string FRAGMENT_SHADER = "/home/liam/Documents/School/5607/hw4/shaders/shader.frag";
bool fullscreen = false;
int SW = 800;
int SH = 600;

int main(int argc, char *argv[]){
    SDL_Window* window = InitAndWindow("HW4", 100, 100, SW, SH);
    cout << "window" << endl;

    cout << "vendor: " << glGetString(GL_VENDOR) << endl;
    cout << "renderer: " << glGetString(GL_RENDERER) << endl;
    cout << "version: " << glGetString(GL_VERSION) << endl;

    GLuint program = LoadShaders(VERTEX_SHADER, FRAGMENT_SHADER);
    glUseProgram(program); //Set the active shader (only one can be used at a time)

    Game* game = new Game(MAP);
    game->setAspectRatio((float) SW / SH);
    game->Init(program);

    SDL_SetRelativeMouseMode(SDL_TRUE);
    unsigned int currTime;
    unsigned int lastTime = SDL_GetTicks();

    SDL_Event event;
    while (true){
        vec3 vel = game->getCameraVel();
        if (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT) break;
            //List of keycodes: https://wiki.libsdl.org/SDL_Keycode - You can catch many special keys
            //Scancode referes to a keyboard position, keycode referes to the letter (e.g., EU keyboards)
            else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE) 
                break; //Exit event loop
            else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_f)
                fullscreen = !fullscreen;
            else if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_a)
                    game->SetCameraVel(vec3(-1, vel.y, vel.z));
                else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_d)
                    game->SetCameraVel(vec3(1, vel.y, vel.z));
                else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_w)
                    game->SetCameraVel(vec3(vel.x, vel.y, -1));
                else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_s)
                    game->SetCameraVel(vec3(vel.x, vel.y, 1));
            }
            else if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_d)
                    game->SetCameraVel(vec3(0, vel.y, vel.z));
                else if (event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_s)
                    game->SetCameraVel(vec3(vel.x, vel.y, 0));
                else if (event.key.keysym.sym == SDLK_e)
                    game->InteractKey();
            }
            else if (event.type == SDL_MOUSEMOTION) {
                // cout << event.motion.xrel << endl;
                // cout << event.motion.yrel << endl;
                float x = event.motion.xrel;
                float y = event.motion.yrel;
                game->UpdateCameraAngle(x, y);
            }
            SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
        }

        currTime = SDL_GetTicks();
        game->Update(((float) currTime - lastTime) / 1000);
        game->Draw(program);
        lastTime = currTime;

        SDL_GL_SwapWindow(window); //Double buffering
    }

    //Clean Up
    glDeleteProgram(program);
    SDL_Quit();
    return 0;
}
