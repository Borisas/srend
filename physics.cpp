#include "physics.h"
#include "core.h"

srend::Physics::~Physics() {
    delete _world;
}

void srend::Physics::init(PhysicsConfig cfg) {

    if ( _world != nullptr ) {
        delete _world;
    }

    b2Vec2 gravity(cfg.gravity.x, cfg.gravity.y);
    _world = new b2World(gravity);
    _world->SetDebugDraw(&_renderer);
    _world->SetContactListener(&_collisions);


    _renderer.SetFlags( b2Draw::e_shapeBit );
    _renderer.init();
}

void srend::Physics::update() {

    _world->Step(srend::deltaTime, _velocityIterations, _positionIterations);
}

void srend::Physics::draw() {

    if ( _debugDrawEnabled ) {
        _world->DebugDraw();
    }
}

b2World* srend::Physics::getWorld() {
    return _world;
}

void srend::Physics::setDebugDrawEnabled(bool e) {
    _debugDrawEnabled = e;
}

b2Body* srend::Physics::createBody(b2BodyDef* def) {
    b2Body* body = _world->CreateBody(def);
    return body;
}


#pragma region DebugDraw


void srend::internalPhysicsDebugRenderer::init() {
    _program = database.program("res/shaders/box2d.vert", "res/shaders/box2d.frag");
}

/// Draw a closed polygon provided in CCW order.
void srend::internalPhysicsDebugRenderer::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    
}
/// Draw a solid closed polygon provided in CCW order.
void srend::internalPhysicsDebugRenderer::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    float verts[16];
    for (int i = 0; i < vertexCount; i++) {
      verts[i*2]   = vertices[i].x;
      verts[i*2+1] = vertices[i].y;
    }
    
    srend::renderer.drawPrimitivePolygon(verts, {color.r,color.g,color.b,color.a * 0.5f}, vertexCount);
    srend::renderer.drawPrimitiveLine(verts, {color.r,color.g,color.b,color.a}, vertexCount);
}
/// Draw a circle.
void srend::internalPhysicsDebugRenderer::DrawCircle(const b2Vec2& center, float radius, const b2Color& color) {

}
/// Draw a solid circle.
void srend::internalPhysicsDebugRenderer::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) {

}
/// Draw a line segment.
void srend::internalPhysicsDebugRenderer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {

}
/// Draw a transform. Choose your own length scale.
/// @param xf a transform.
void srend::internalPhysicsDebugRenderer::DrawTransform(const b2Transform& xf) {

}
/// Draw a point.
void srend::internalPhysicsDebugRenderer::DrawPoint(const b2Vec2& p, float size, const b2Color& color) {

}


#pragma endregion DebugDraw

#pragma region Collisions

void srend::internalPhysicsCollisions::BeginContact(b2Contact* contact) {

    auto bodyA = contact->GetFixtureA()->GetBody();
    auto bodyB = contact->GetFixtureB()->GetBody();

    auto pointerA = bodyA->GetUserData().pointer;
    auto pointerB = bodyB->GetUserData().pointer;

    if ( pointerA->node->isEnabled() == false || pointerB->node->isEnabled() == false )
        return;//dont pass to listener

    pointerA->listener->onCollisionBegin(contact, pointerB);
    pointerB->listener->onCollisionBegin(contact, pointerA);

}

void srend::internalPhysicsCollisions::EndContact(b2Contact* contact) {

    auto bodyA = contact->GetFixtureA()->GetBody();
    auto bodyB = contact->GetFixtureB()->GetBody();

    auto pointerA = bodyA->GetUserData().pointer;
    auto pointerB = bodyB->GetUserData().pointer;
    
    if ( pointerA->node->isEnabled() ) {
        pointerA->listener->onCollisionEnd(contact, pointerB);
    }
    
    if ( pointerB->node->isEnabled() ) {
        pointerB->listener->onCollisionEnd(contact, pointerA);
    }
}

#pragma endregion Collisions