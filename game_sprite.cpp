#include "game_sprite.h"
#include "core.h"

srend::GameSprite::GameSprite() { }
srend::GameSprite::~GameSprite() { }

void srend::GameSprite::setup() {
    srend::GameNode::setup();
    loadProgramDefault();
}

srend::GameSprite* srend::GameSprite::create() {
    if(srend::_scene == nullptr) 
        return nullptr;
    auto ret = (GameSprite*)srend::_scene->assignNode(new GameSprite());
    ret->setup();
    return ret;
}

void srend::GameSprite::loadTexture(std::string texture) {
    _sprite.texture = srend::database.image(texture);
    if ( !_anyProgramLoaded ) {
        loadProgramDefault();
    }
}

void srend::GameSprite::loadProgramDefault() {
    loadProgram("res/shaders/default.vert", "res/shaders/default.frag");
}

void srend::GameSprite::loadProgram(std::string vertShader, std::string fragShader) {
    _sprite.program = database.program(vertShader, fragShader);
}

void srend::GameSprite::draw() {

    srend::renderer.draw(
        _sprite.program->program, 
        _sprite.texture, 
        transform
    );
}

srend::float2 srend::GameSprite::getSize() {
    return srend::float2(
        _sprite.texture->width,
        _sprite.texture->height
    );
}
