#ifndef SRC_INCLUDE_UTILS_H_
#define SRC_INCLUDE_UTILS_H_

#include <GL/glew.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace glm;
using namespace std;

enum class GAME_IDS : int {
    EMPTY = 0,
    WALL,
    PLAYER,
    DOOR,
    KEY,
};

inline void CreateNormals(float normals[], float verts[], int num_verts) {
    float cube_normals[36 * 3];
    for (int i = 0; i < num_verts*3; i += 9) {
        float ax = verts[i + 0];
        float ay = verts[i + 1];
        float az = verts[i + 2];
        float bx = verts[i + 3];
        float by = verts[i + 4];
        float bz = verts[i + 5];
        float cx = verts[i + 6];
        float cy = verts[i + 7];
        float cz = verts[i + 8];
        vec3 AB(bx - ax, by - ay, bz - az);
        vec3 AC(cx - ax, cy - ay, cz - az);
        vec3 n = normalize(cross(AB, AC));
        for (int j = 0; j < 3; j++) {
            normals[i + 3*j + 0] = n.x;
            normals[i + 3*j + 1] = n.y;
            normals[i + 3*j + 2] = n.z;
        }
    }
}

inline SDL_Window* InitAndWindow(string title, int ox, int oy, int w, int h) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cout << "Failed to init SDL" << endl;
        return nullptr;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    SDL_Window* window = SDL_CreateWindow(title.c_str(), ox, oy, w, h, SDL_WINDOW_OPENGL);
    if (window == NULL) {
        cout << "Failed to create an SDL2 window" << endl;
        exit(1);
    }
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context == NULL) {
        cout << "Failed to create an opengl context" << endl;
        exit(1);
    }

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cout << "Failed to init GLEW" << endl;
        exit(1);
    }
    if (SDL_GL_SetSwapInterval(1) < 0)
        cout << "Failed to set vsync" << endl;

    glEnable(GL_DEPTH_TEST);

    return window;
}

inline GLuint LoadShaders(string vertex_file_path, string fragment_file_path) {
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    string VertexShaderCode;
    ifstream VertexShaderStream(vertex_file_path, ios::in);
    if(VertexShaderStream.is_open()){
        string Line = "";
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    } else {
        cerr << "Could not open vertex shader: " << vertex_file_path << endl;
        exit(1);
    }

    // Read the Fragment Shader code from the file
    string FragmentShaderCode;
    ifstream FragmentShaderStream(fragment_file_path, ios::in);
    if(FragmentShaderStream.is_open()){
        string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    } else {
        cerr << "Could not open fragment shader: " << fragment_file_path << endl;
        exit(1);
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    // printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    // printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }



    // Link the program
    // printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }


    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

#endif  // SRC_INCLUDE_UTILS_H_
