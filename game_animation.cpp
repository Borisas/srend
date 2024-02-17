#include "game_animation.h"
#include "core.h"

using namespace srend;

GameAnimation* GameAnimation::create() {
    if(srend::_scene == nullptr) 
        return nullptr;
    auto ret = (GameAnimation*)srend::_scene->assignNode(new GameAnimation());
    ret->setup();
    return ret;
}

void srend::GameAnimation::setup() {
    onEnable += srend::Action::bind(&GameAnimation::_onSelfEnable, this);
}

void srend::GameAnimation::loadAnimation(std::string file, int frameWidth, int frameHeight) {

    _sprite.texture = srend::database.image(file);
    _frameWidth = frameWidth;
    _frameHeight = frameHeight;
    _calculatedTotalFrames = -1;
}

void GameAnimation::loadProgramDefault() {
  loadProgram("res/shaders/default.vert", "res/shaders/default_animation.frag");
}

void GameAnimation::loadProgram(std::string vertShader, std::string fragShader) {
    _sprite.program = database.program(vertShader, fragShader);
}

void srend::GameAnimation::setFrameRate(int fr) {
    _frameRate = fr;
}

void srend::GameAnimation::setOverrideTotalFrames(int ot) {
    _overrideTotalFrames = ot;
}

void srend::GameAnimation::setLoops(int l) {
    _loops = l;
}

void srend::GameAnimation::setStopAction(srend::AnimationStopAction ac) {
    _stopAction = ac;
}

void srend::GameAnimation::draw() {

    auto uv = _calculateFrameUv();

    srend::renderer.draw(
        _sprite.program->program,
        _sprite.texture,
        uv,
        transform,
        _frameWidth,
        _frameHeight
    );
}

void srend::GameAnimation::update() {

    _animTimer += srend::deltaTime;

    if ( _animTimer >= 1.0f / (float)_frameRate) {

        int tf = _totalFrames()-1;

        if ( _frameIndex < tf ) {
            _frameIndex++;
        }
        else if ( _frameIndex >= tf ) {
            if ( _loops < 0 || _loopsCompleted < _loops ) {
                while ( _frameIndex >= tf ) {
                    _frameIndex -= tf;
                }
            }
            else {
                _performStopAction();
            }
            onFinished.invoke();
        }

        _animTimer = 0.0f;
    }

}

void srend::GameAnimation::_onSelfEnable() {
    _loopsCompleted = 0;
    _frameIndex = 0;
}

int srend::GameAnimation::_totalFrames() {
  if (_overrideTotalFrames > 0)
    return _overrideTotalFrames;
  if (_calculatedTotalFrames <= 0) {
    // calculate

    float w = _sprite.texture->width;
    float h = _sprite.texture->height;

    int x = w / _frameWidth;
    int y = h / _frameHeight;

    y = y <= 0 ? 1 : y;
    x = x <= 0 ? 1 : x;

    _calculatedTotalFrames = x * y;
  }
  return _calculatedTotalFrames;
}

float4 srend::GameAnimation::_calculateFrameUv() {

    float4 uv;
    int framesInRow = _sprite.texture->width / _frameWidth;

    int y = _frameIndex / framesInRow;
    int x = _frameIndex - y * framesInRow;

    uv.x = (float)x * _frameWidth;
    uv.y = (float)y * _frameHeight;
    uv.z = uv.x + _frameWidth;
    uv.w = uv.y + _frameHeight;

    uv.x /= _sprite.texture->width;
    uv.y /= 1.0f - _sprite.texture->height;
    uv.z /= _sprite.texture->width;
    uv.w /= 1.0f - _sprite.texture->height;

    return uv;
}

void srend::GameAnimation::_performStopAction() {

    switch(_stopAction){
        case AnimationStopAction::NONE:
        break;
        case AnimationStopAction::DISABLE:
            setEnabled(false);
        break;
        case AnimationStopAction::DESTROY:
            destroy();
        break;
    }

}
