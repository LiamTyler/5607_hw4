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

ostream& operator<<(ostream& out, const mat4& m) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            out << m[r][c] << " ";
        }
        out << endl;
    }
}

ostream& operator<<(ostream& out, const vec4& m) {
    for (int r = 0; r < 4; r++)
        out << m[r] << " ";
    out << endl;
}

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
    game->Init();

    //Event Loop (Loop forever processing each event as fast as possible)
    SDL_Event windowEvent;
    while (true){
        if (SDL_PollEvent(&windowEvent)){
            if (windowEvent.type == SDL_QUIT) break;
            //List of keycodes: https://wiki.libsdl.org/SDL_Keycode - You can catch many special keys
            //Scancode referes to a keyboard position, keycode referes to the letter (e.g., EU keyboards)
            if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE) 
                break; //Exit event loop
            if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_f)
                fullscreen = !fullscreen;
            SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
        }

        game->Draw(program);

        SDL_GL_SwapWindow(window); //Double buffering
    }

    //Clean Up
    glDeleteProgram(program);
    SDL_Quit();
    return 0;
}
