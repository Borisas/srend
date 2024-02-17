#include "core.h"

int srend::_screenWidth;
int srend::_screenHeight;
const char* srend::_windowTitle;
double srend::deltaTime;
SDL_Window* srend::_window;
SDL_GLContext srend::_context;
GLuint srend::_gProgramId;
GLuint srend::_texture;
srend::Renderer srend::renderer;
srend::Database srend::database;
srend::Scene* srend::_scene;
srend::Input srend::input;
srend::Physics srend::physics;
srend::Random srend::rng;
srend::TweenCore srend::_tweener;

bool srend::init(const char* windowTitle, int screenWidth, int screenHeight) {

    _windowTitle = windowTitle;
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;
    rng = srend::Random();

    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        SYSLOG( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		return false;
    }

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    _window = SDL_CreateWindow(
        _windowTitle,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        _screenWidth,
        _screenHeight,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    if ( _window == NULL ) {
        SYSLOG( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
    }

    _context = SDL_GL_CreateContext(_window);
    if ( _context == NULL ) {
        SYSLOG( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
    }

    auto glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if ( glewError != GLEW_OK ) {
        SYSLOG( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
        return false;
    }

    if ( SDL_GL_SetSwapInterval(1) < 0 ) {
        SYSLOG( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
    }

    if ( !initShaders() ) {
        SYSLOG("Unable to initialize shaders\n");
        return false;
    }

    if ( !initGL() ) {
        SYSLOG( "Unable to initialize OpenGL!\n" );
        return false;
    }

    if ( !database.init() ) {

        SYSLOG( "Unable to initialize Database!!\n" );
        return false;
    }


    return true;
}
    

bool srend::initShaders() {
    
    auto p = database.program("res/shaders/default.vert","res/shaders/default.frag");
    if ( p == nullptr ) return false;

    _gProgramId = p->program;
    return true;
}

bool srend::initGL() {

    glDisable( GL_DEPTH_TEST );
    glClearColor(0.f,0.f,0.f,1.f);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //for fonts?

    glViewport( 0, 0, _screenWidth, _screenHeight );

    renderer.init(_screenWidth,_screenHeight,1.f,1.f);

    return true;
}

void srend::run() {

    SDL_Event e;
    bool quit = false;
    while( !quit ) {
        Uint64 tstart = SDL_GetTicks64();
        input.frameStart();
        while( SDL_PollEvent( &e ) != 0 ) {
            input.process(e);
            if( e.type == SDL_QUIT ) {
                quit = true;
            }
        }
        
        srend::physics.update();
        srend::update();
        srend::lateUpdate();
        srend::render();
        if ( _scene != nullptr ) {
            _scene->frameCompleted();
        }
        Uint64 tend = SDL_GetTicks64();
        deltaTime = (tend-tstart)/1000.0;
    }
}

void srend::update() {
    if ( _scene != nullptr ) {
        _tweener.update();
        _scene->update();
    }
}

void srend::lateUpdate() {
    if ( _scene != nullptr ) {
        _scene->lateUpdate();
    }
}

void srend::render() {

    glClear(GL_COLOR_BUFFER_BIT);

    if ( _scene != nullptr ) {
        _scene->draw();
    }

    physics.draw();
    renderer.drawPrimitives();

    // glUseProgram(NULL);
    SDL_GL_SwapWindow( _window );
    SDL_Delay( 1 );
}

void srend::exit() {
    SDL_DestroyWindow(_window);
    _window = NULL;

    SDL_Quit();
}

void srend::loadScene(srend::Scene* s) {
    if ( _scene != nullptr ) {
        _scene->unload();
    }

    _scene = s;
    _scene->load();
}

void srend::traverseNodeTree(GameNode* n, std::function<void(GameNode*)> call) {

    call(n);
    for(int i = 0; i < n->getChildCount(); i++ ) {
        auto child = n->getChild(i);
        call(child);
        traverseNodeTree(child, call);
    }
}