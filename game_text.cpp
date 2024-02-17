#include "game_text.h"
#include "core.h"
#include "database.h"

srend::GameText* srend::GameText::create() {
    if(srend::_scene == nullptr) 
        return nullptr;
    auto ret = (GameText*)srend::_scene->assignNode(new GameText());
    ret->setup();
    return ret;
}

void srend::GameText::setup() {
    srend::GameNode::setup();
    loadProgramDefault();
}

void srend::GameText::loadFont(std::string fontFile) {
    _font = srend::database.font(fontFile);
    setSize(16);
}

void srend::GameText::loadProgramDefault() {
    loadProgram("res/shaders/default.vert", "res/shaders/default_text.frag");
}

void srend::GameText::loadProgram(std::string vertShader, std::string fragShader) {
    _program = database.program(vertShader, fragShader);
}

void srend::GameText::draw() {

    _font->setSize(_localSize);
    srend::renderer.draw(
        _program->program,
        _font,
        _text,
        _color,
        transform,
        horizontalAlignment,
        verticalAlignment
    );
}

void srend::GameText::setText(const std::string& text) {
    if ( _text == text ) return;
    _text = text;
    _sizeCacheDirty = true;
}

void srend::GameText::setSize(int sz) {
    _localSize = sz;
}

void srend::GameText::setColor(const srend::float4& c) {
    _color = c;
}

float srend::GameText::getWidth() {
    if ( _sizeCacheDirty ) {
        _calculateSizeCache();
    }
    return _cachedWidth;
}

float srend::GameText::getHeight() {
    if ( _sizeCacheDirty ) {
        _calculateSizeCache();
    }
    return _cachedHeight;
}

void srend::GameText::_calculateSizeCache() {

    _font->setSize(_localSize);

    float w = 0.0f;
    float h = 0.0f;

    std::vector<float> wLines;
    std::vector<float> hLines;
    int line = 0;

    for ( int i = 0; i < _text.size(); i++ ) {
        const char& c = _text[i];
        if ( c == '\n' ) {
            line++;
            continue;
        }
        auto ch = _font->character(c);
        if ( wLines.size() <= line ) wLines.push_back(0);
        if ( hLines.size() <= line ) hLines.push_back(0);

        wLines[line] += (ch->advance >> 6);
        float szy = (ch->size.y);
        hLines[line] = szy > hLines[line] ? szy : hLines[line];
    }

    float maxLineHeight = 0.0f;
    for ( int i = 0; i < hLines.size(); i++ ) {
        if ( maxLineHeight < hLines[i] ) maxLineHeight = hLines[i];
    }
    maxLineHeight *= 1.1f;
    float maxLineWidth = 0.0f;
    for ( int i = 0; i < wLines.size(); i++ ) {
        if ( maxLineWidth < wLines[i] ) maxLineWidth = wLines[i];
    }

    _cachedWidth = maxLineWidth;
    _cachedHeight = maxLineHeight * hLines.size();
}
