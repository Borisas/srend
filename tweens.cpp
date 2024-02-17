#include "tweens.h"
#include "core.h"

using namespace srend::Tween;

Value* srend::Tween::Value::create(float from, float to, float t,std::function<void(float)> onStep) {
    auto ret = (Value*)srend::_tweener.assign(new Value());
    ret->step = onStep;
    ret->_duration = t;
    ret->_from = from;
    ret->_to = to;
    return ret;
}

void srend::Tween::Value::update() {

    step(srend::math::lerp(_from,_to,getT()));
    _time += srend::deltaTime;
}
