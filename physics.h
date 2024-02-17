#pragma once
#include "box2d/box2d.h"
#include "types.h"
#include <vector>

namespace srend {

    class GameNode;

    struct PhysicsConfig {
        float2 gravity;
    };

    class internalPhysicsDebugRenderer : public b2Draw {

        public:
            void init();

            /// Draw a closed polygon provided in CCW order.
            void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
            /// Draw a solid closed polygon provided in CCW order.
            void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
            /// Draw a circle.
            void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;
            /// Draw a solid circle.
            void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override;
            /// Draw a line segment.
            void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
            /// Draw a transform. Choose your own length scale.
            /// @param xf a transform.
            void DrawTransform(const b2Transform& xf) override;
            /// Draw a point.
            void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;

        private:
            GLProgram* _program;

    };

    class internalPhysicsCollisions : public b2ContactListener {
        
        public:
            void BeginContact(b2Contact*) override;
            void EndContact(b2Contact*) override;
    };

    class Physics {

        public:

            ~Physics();

            void init(PhysicsConfig cfg);
            void update();
            void draw();

            b2World* getWorld();

            void setDebugDrawEnabled(bool);

            b2Body* createBody(b2BodyDef*);

        private:

            int _velocityIterations = 6;
            int _positionIterations = 2;
            bool _debugDrawEnabled = false;


            b2World* _world = nullptr;
            internalPhysicsCollisions _collisions;
            internalPhysicsDebugRenderer _renderer;
    };
}