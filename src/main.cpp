#include <iostream>
#include <vector>
#include "include/map.h"

using namespace std;

string MAP = "/home/liam/Documents/School/5607/hw4/maps/base.txt";
string VERTEX_SHADER = "/home/liam/Documents/School/5607/hw4/shaders/shader.vert";
string FRAGMENT_SHADER = "/home/liam/Documents/School/5607/hw4/shaders/shader.frag";
bool fullscreen = false;
int SW = 800;
int SH = 600;

static const GLfloat cube_vertices[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

int main(int argc, char *argv[]){
    SDL_Window* window = InitAndWindow("HW4", 100, 100, SW, SH);

    cout << "vendor: " << glGetString(GL_VENDOR) << endl;
    cout << "renderer: " << glGetString(GL_RENDERER) << endl;
    cout << "version: " << glGetString(GL_VERSION) << endl;

    GLuint program = LoadShaders(VERTEX_SHADER, FRAGMENT_SHADER);
    glUseProgram(program); //Set the active shader (only one can be used at a time)

    // Set up Cube vao / vbo
	GLuint cube_vao;
	glGenVertexArrays(1, &cube_vao);
	glBindVertexArray(cube_vao);

	GLuint cube_vbo;
	glGenBuffers(1, &cube_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

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

        // Clear the screen to default color
		glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Get model, view, proj posititions in GPU
        GLint uniModel = glGetUniformLocation(program, "model");
        GLint uniView  = glGetUniformLocation(program, "view");
        GLint uniProj  = glGetUniformLocation(program, "proj");

        // create the model matrix
        mat4 model(1.0f);
        model = scale(model, vec3(10, 10, 10));
        model = translate(model, vec3(0, 0, -5));

        // create the view matrix
        mat4 view = lookAt(
                vec3(0.f, 0.f, 10.0f),
                vec3(0.f, 0.f, 0.f),
                vec3(0.f, 1.0f, 0.f));

        // create the projection matrix
        mat4 proj = perspective(radians(45.0f), (float) SW / SH, .1f, 100.0f);

        // Send the model, view, and proj matrices to the GPU
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(uniView, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(uniProj, 1, GL_FALSE, &proj[0][0]);

        // Actually draw the cube
        glBindVertexArray(cube_vao);
		glDrawArrays(GL_TRIANGLES, 0, 12*3);

        SDL_GL_SwapWindow(window); //Double buffering
    }


    //Clean Up
    glDeleteProgram(program);
    SDL_Quit();
    return 0;
}
