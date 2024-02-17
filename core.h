#pragma once

#include "SDL2/SDL.h"
#include "GL/glew.h"
#include "SDL2/SDL_opengl.h"
#include "OpenGL/glu.h"
#include "renderer.h"
#include "database.h"
#include "types.h"
#include "scene.h"
#include "input.h"
#include "game_node.h"
#include "game_animation.h"
#include "game_sprite.h"
#include "game_button.h"
#include "game_text.h"
#include "physics.h"
#include "mathf.h"
#include "event.h"
#include "random.h"
#include "tween_core.h"
#include "tweens.h"
#include <stdio.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <vector>

#define SYSLOG printf
#define fCREATE(x) \
static x* create() {if(srend::_scene == nullptr) return nullptr;auto ret = (x*)srend::_scene->assignNode(new x()); ret->setup(); return ret;}

namespace srend {

    extern int _screenWidth;
    extern int _screenHeight;
    extern const char* _windowTitle;
    extern double deltaTime;

    extern SDL_Window* _window;
    extern SDL_GLContext _context;

    extern GLuint _gProgramId;
    extern GLuint _texture;

    extern Renderer renderer;
    extern Database database;
    extern Input input;
    extern Physics physics;
    extern Random rng;
    extern TweenCore _tweener;

    extern srend::Scene* _scene;

    bool init(const char* windowTitle, int screenWidth, int screenHeight);
    bool initWorkingDir();
    bool initShaders();
    bool initGL();
    void loadScene(srend::Scene*);
    void run();
    void update();
    void lateUpdate();
    void render();
    void exit();

    void traverseNodeTree(GameNode*, std::function<void(GameNode*)>);

}