#include "types.h"
#include "core.h"
#include "game_node.h"
#include "event.h"
#include "mathf.h"

const std::array<float,9> & srend::Transform::getMat3() {

    if ( _isDirty() ) {
        auto c = cos(rotation * DEG_2_RAD);
        auto s = sin(rotation * DEG_2_RAD);

        auto tm = std::array<float,9>({
            1.f,0.f,position.x,
            0.f,1.f,position.y,
            0.f,0.f,1.f 
        });

        auto sm = std::array<float,9>({
            scale.x,0.f,0.f,
            0.f,scale.y,0.f,
            0.f,0.f,1.f,
        });

        auto rm = std::array<float,9>({
            c, -s,  0.f,
            s,  c,  0.f,
            0.f,0.f,1.f
        });

        _cachedMat = math::mul3x3(math::mul3x3(tm,rm),sm);
        if ( _owner != nullptr && _owner->getParent() != nullptr ) {
            auto parentMat = _owner->getParent()->transform.getMat3();
            _cachedMat = math::mul3x3(parentMat, _cachedMat);
        }
        _updateCache();
        _calculateIMat3();
        onUpdated();
    }
    return _cachedMat;
}

const srend::mat3& srend::Transform::getIMat3() {
    getMat3();
    return _cachedIMat;
}

void srend::Transform::forceDirty() {
    _dirtyForced = true;
}

void srend::Transform::setOwner(srend::GameNode* owner) {
    _owner = owner;
}

srend::float2 srend::Transform::worldPosition() {

    if ( _owner->getParent() == nullptr ) {
        return position;
    }
    else {
        return srend::math::mul(_owner->getParent()->transform.getMat3(),position);
    }
}

void srend::Transform::worldPosition(srend::float2 wp) {
    if ( _owner->getParent() == nullptr ) {
        position = wp;
    }
    else {
        position = srend::math::mul(_owner->getParent()->transform.getIMat3(),wp);
    }
}

bool srend::Transform::_isDirty() {
    return  _dirtyForced ||
            abs(_cachedPosition.x - position.x) > FLT_EPSILON ||
            abs(_cachedPosition.y - position.y) > FLT_EPSILON ||
            abs(_cachedRotation - rotation) > FLT_EPSILON ||
            abs(_cachedScale.x - scale.x) > FLT_EPSILON ||
            abs(_cachedScale.y - scale.y) > FLT_EPSILON;

}

void srend::Transform::_updateCache() {
    _cachedPosition = position;
    _cachedRotation = rotation;
    _cachedScale = scale;
}

void srend::Transform::_calculateIMat3() {
    _cachedIMat = math::inverse3x3(_cachedMat);
}

srend::PhysicsBody::~PhysicsBody() {
    if ( shape != nullptr ) {
        delete shape;
    }
    body->DestroyFixture(fixture);
    body->GetWorld()->DestroyBody(body);
}

void srend::PhysicsBody::setup(srend::GameNode* n, srend::IPhysicsListener* l) {
    setup(n,l,b2_dynamicBody);
}

void srend::PhysicsBody::setup(srend::GameNode* n, srend::IPhysicsListener* l, b2BodyType bodyType) {
    node = n;
    listener = l;
    bodyDef.type = bodyType;
    body = srend::physics.createBody(&bodyDef);
    body->GetUserData().pointer = this;
    body->SetGravityScale(0.f);

    n->onDisable += srend::Action::bind(&srend::PhysicsBody::_onOwnerDisabled, this);
    n->onEnable += srend::Action::bind(&srend::PhysicsBody::_onOwnerEnabled, this);
}

void srend::PhysicsBody::makeRectShape(float w, float h) {
    if ( body == nullptr ) return;

    auto ts = new b2PolygonShape();
    ts->SetAsBox(w,h);
    shape = ts;

    body->CreateFixture(shape,0.0f);
}

void srend::PhysicsBody::updateBodyTransform() {
    
    const Transform& t = node->transform;

    body->SetTransform(t.position.b2(),t.rotation * DEG_2_RAD);
}

void srend::PhysicsBody::updateNodeTransform() {

    auto t = body->GetTransform();
    
    node->transform.position.x = t.p.x;
    node->transform.position.y = t.p.y;
    node->transform.rotation = t.q.GetAngle() * RAD_2_DEG;
}

void srend::PhysicsBody::_onNodeEnableChanged(bool e) {
    changeEnabled = e ? 1 : 0;
}

void srend::PhysicsBody::_updateState() {
    if ( changeEnabled != -1 ) {
        body->SetEnabled(changeEnabled == 1 ? true : false);
        changeEnabled = -1;
    }
}

void srend::PhysicsBody::_onOwnerDisabled() {
    if ( body != nullptr ) {
        body->SetEnabled(false);  
    }
}

void srend::PhysicsBody::_onOwnerEnabled() {
    if ( body != nullptr ) {
        body->SetEnabled(true);
    }
}

void srend::Font::setSize(int sz) {
    sz = srend::math::clamp(1,999,sz);

    if ( _lastSize == sz ) return;
    FT_Set_Pixel_Sizes(face, 0, sz);  
    _lastSize = sz;
}

srend::FontChar* srend::Font::character(char c) {

    auto hash = _getHash(_lastSize,c);

    if ( _charCache.count(hash) != 0 ) {
        return &_charCache[hash];
    }

    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
        // std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        // continue;
        return nullptr;
    }
    // generate texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer
    );
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // now store character for later use
    srend::FontChar character = {
        texture, 
        srend::int2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        srend::int2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        face->glyph->advance.x
    };
    _charCache[hash] = character;
    return &_charCache[hash];
}

void srend::Font::release() {
  // release textures
    FT_Done_Face(face);
}

int srend::Font::_getHash(int sz, char c) {
    return (int)c * 1000 + sz; // only really works cuz size is clamped to [1;999]
}

std::array<float, 4> srend::float4::array() const {
  return std::array<float, 4>({
    x,y,z,w
  });
}
