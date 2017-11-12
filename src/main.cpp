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
    1.0f,-1.0f, 1.0f, // tri 3 b
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f, // tri 3 e
    1.0f, 1.0f,-1.0f, // tri 4 b
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f, // tri 4 e
    -1.0f,-1.0f,-1.0f, // tri 5 b
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f, // tri 5 e
    1.0f,-1.0f, 1.0f, // tri 6 b
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f, // tri 6 e
    -1.0f, 1.0f, 1.0f, // tri 7 b
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f, // tri 7 e
    1.0f, 1.0f, 1.0f, // tri 8 b
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f, // tri 8 e
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
    float cube_normals[36 * 3];
    for (int i = 0; i < 12; ++i) {
        float ax = cube_vertices[9*i + 0];
        float ay = cube_vertices[9*i + 1];
        float az = cube_vertices[9*i + 2];
        float bx = cube_vertices[9*i + 3];
        float by = cube_vertices[9*i + 4];
        float bz = cube_vertices[9*i + 5];
        float cx = cube_vertices[9*i + 6];
        float cy = cube_vertices[9*i + 7];
        float cz = cube_vertices[9*i + 8];
        vec3 AB(bx - ax, by - ay, bz - az);
        vec3 AC(cx - ax, cy - ay, cz - az);
        vec3 n = normalize(cross(AB, AC));
        for (int j = 0; j < 3; j++) {
            cube_normals[9*i + 3*j + 0] = n.x;
            cube_normals[9*i + 3*j + 1] = n.y;
            cube_normals[9*i + 3*j + 2] = n.z;
        }
    }

    for (int i = 0; i < 36; i++) {
        cout << cube_normals[3*i + 0] << " ";
        cout << cube_normals[3*i + 1] << " ";
        cout << cube_normals[3*i + 2] << " ";
        cout << endl;
    }

    SDL_Window* window = InitAndWindow("HW4", 100, 100, SW, SH);
    cout << "window" << endl;

    cout << "vendor: " << glGetString(GL_VENDOR) << endl;
    cout << "renderer: " << glGetString(GL_RENDERER) << endl;
    cout << "version: " << glGetString(GL_VERSION) << endl;

    GLuint program = LoadShaders(VERTEX_SHADER, FRAGMENT_SHADER);
    glUseProgram(program); //Set the active shader (only one can be used at a time)

    // Set up Cube vao / vbo
	GLuint cube_vao;
	glGenVertexArrays(1, &cube_vao);
	glBindVertexArray(cube_vao);

	GLuint cube_verts_vbo;
	glGenBuffers(1, &cube_verts_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cube_verts_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint cube_normals_vbo;
	glGenBuffers(1, &cube_normals_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cube_normals_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_normals), cube_normals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);



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
        model = scale(model, vec3(5, 5, 5));
        // model = translate(model, vec3(0, 0, -5));

        // create the view matrix
        mat4 view = lookAt(
                vec3(50.f, 20.f, 50.0f),
                vec3(0.f, 0.f, 0.f),
                vec3(0.f, 1.0f, 0.f));

        // create the projection matrix
        mat4 proj = perspective(radians(45.0f), (float) SW / SH, .1f, 100.0f);

        // Send the model, view, and proj matrices to the GPU
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(uniView, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(uniProj, 1, GL_FALSE, &proj[0][0]);

        // light
        vec3 lightDir = vec3(-1.0, 0, 0);
        GLint uniLight = glGetUniformLocation(program, "lightInEyeSpace");
        vec4 l = view * vec4(lightDir, 0);
        glUniform4f(uniLight, l[0], l[1], l[2], l[3]);

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
