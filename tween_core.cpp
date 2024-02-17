#include "tween_core.h"
#include "mathf.h"
#include "core.h"

void srend::TweenCore::update() {

    int i = 0;
    for ( int i = 0; i < _tweens.size(); i++ ) {
        auto & tw = _tweens[i];
        if ( tw->isCompleted() || tw->isKilled() ) {
            _tweens.erase(_tweens.begin()+i);
            i--;
        }
        else {
            tw->update();
        }
    }
}

srend::TweenBase* srend::TweenCore::assign(srend::TweenBase* tween) {
    _tweens.push_back(std::unique_ptr<TweenBase>(tween));
    return _tweens[_tweens.size()-1].get();
}

srend::TweenBase* srend::TweenCore::assign(std::unique_ptr<srend::TweenBase> tween) {
    _tweens.push_back(std::move(tween));
    return _tweens[_tweens.size()-1].get();
}

bool srend::TweenCore::isValid(TweenBase* ptr) {
    for ( int i = 0; i < _tweens.size(); i++ ) {
        if ( _tweens[i].get() == ptr ) return true;
    }
    return false;
}

void srend::TweenBase::kill() {
    _killed = true;
}

float srend::TweenBase::getT() {
    return srend::math::clamp(0.0f,1.0f, _time/_duration);
}

bool srend::TweenBase::IsValid(srend::TweenBase* ptr) {
    return srend::_tweener.isValid(ptr);
}

srend::TweenBase::TweenBase() {}

srend::TweenBase::~TweenBase() {}

bool srend::TweenBase::isCompleted() {
  return _time >= _duration;
}

bool srend::TweenBase::isKilled() {
  return _killed;
}
