#include "game_button.h"

#include "core.h"
#include "mathf.h"

void srend::GameButton::update() {

    auto mpos = srend::input.mousePosition();

    if ( _hovered ) {
        onHoverExit();
        _hovered = false;
    }

    if ( !_isMouseOver(mpos) ) {
        return;
    }

    if ( srend::input.mouseUp(left) && _clicked ) {
        onRelease();
        _clicked = false;
    }

    if ( srend::input.mouseDown(left) && !_clicked ) {
        onClick();
        _clicked = true;
    }

    if ( srend::input.mouseUp(right) ) {
        onContext();
    }

    if ( !_hovered ) {
        onHoverEnter();
        _hovered = true;
    }
}

void srend::GameButton::draw() {
    if ( _drawn == false ) return;
    srend::GameSprite::draw();
}

void srend::GameButton::onHoverEnter() {}

void srend::GameButton::onHoverExit() {}

void srend::GameButton::onClick() {
    transform.scale = srend::float2(0.975f,0.975f);
}

void srend::GameButton::onRelease() {
    transform.scale = srend::float2(1.0f,1.0f);
}

void srend::GameButton::onContext() {}

void srend::GameButton::overrideWidth(int w) {
    _width = w;
}

void srend::GameButton::overrideHeight(int h) {
    _height = h;
}

int srend::GameButton::width() {
    if ( _width >= 0 ) {
        return _width;
    }
    return _sprite.texture->width;
}

int srend::GameButton::height() {
    if ( _height >= 0 ) {
        return _height;
    }
    return _sprite.texture->height;
}

srend::float2 srend::GameButton::getSize() {
    return srend::float2(
        width(),
        height()
    );
}

void srend::GameButton::setDrawn(bool d) {
    _drawn = d;
}

bool srend::GameButton::_isMouseOver(const float2& mpos) {

    auto mat = transform.getIMat3();
    auto plocal1 = mpos;

    plocal1 = srend::math::mul(mat, plocal1);

    float hwidth = width() * 0.5f;
    float hheight = height() * 0.5f;

    return abs(plocal1.x) <= hwidth && abs(plocal1.y) <= hheight;
}
