#pragma once

#include "SDL2/SDL.h"
#include "GL/glew.h"
#include "SDL2/SDL_opengl.h"
#include "OpenGL/glu.h"
#include "box2d/box2d.h"
#include <string>
#include <array>
#include <vector>
#include <math.h>
#include <functional>

#include "ft2build.h"
#include FT_FREETYPE_H  

namespace srend {

    class GameNode;
    typedef std::array<float,9> mat3;
    typedef std::array<float,6> pfloat6;

    struct Texture {
        int width;
        int height;
        int nChannels;
        GLuint data;
    };

    struct GLProgram {
        GLuint program;
        std::string name;
    };

    struct Sprite {
        GLProgram* program;
        Texture* texture;
    };


    struct color4 {

        color4(float r, float g, float b, float a):r(r),g(g),b(b),a(a){};

        float r;
        float g;
        float b;
        float a;
    };

    struct float2 {

        float2() : x(0.0f),y(0.0f){}
        float2(float x, float y) : x(x), y(y){}

        float x;
        float y;

        std::array<float, 2> get() const {
            std::array<float,2> v;
            v[0] = x;
            v[1] = y;
            return v;
        }

        b2Vec2 b2() const {
            return b2Vec2(x,y);
        }
    };

    struct int2 {
        int2():x(0),y(0){};
        int2(int x, int y):x(x),y(y){};
        int x;
        int y;
    };

    struct float3 {
        float3():x(0.0f),y(0.0f),z(0.0f){};
        float3(float x, float y, float z):x(x),y(y),z(z){};
        float x;
        float y;
        float z;
    };

    struct float4 {
        float4():x(0.0f),y(0.0f),z(0.0f),w(0.0f){}
        float4(float x, float y, float z, float w):x(x),y(y),z(z),w(w){}

        float x;
        float y;
        float z;
        float w;

        std::array<float,4> array() const;
    };

    enum struct VerticalAlignment {
        TOP = 0,
        CENTER = 1,
        BOTTOM = 2
    };

    enum struct HorizontalAlignment {
        LEFT = 0,
        CENTER = 1,
        RIGHT = 2
    };

    struct Transform {
        float2 position;
        float rotation;
        float2 scale;

        Transform() {

            scale.x = 1.0f;
            scale.y = 1.0f;
        }

        const mat3 & getMat3();
        const mat3 & getIMat3();

        std::function<void()> onUpdated;
        void forceDirty();
        void setOwner(srend::GameNode*);

        float2 worldPosition();
        void worldPosition(float2 wp);

        private:

        srend::GameNode* _owner;
        bool _dirtyForced = false;
        bool _isDirty();
        void _updateCache();
        void _calculateIMat3();
        mat3 _cachedMat;
        mat3 _cachedIMat;
        float2 _cachedPosition;
        float _cachedRotation;
        float2 _cachedScale;
    };

    class IPhysicsListener {
        public:
        virtual void onCollisionBegin(b2Contact* contact, PhysicsBody* other) = 0;
        virtual void onCollisionEnd(b2Contact* contact, PhysicsBody* other) = 0;
    };

    struct PhysicsBody {

        ~PhysicsBody();

        srend::GameNode* node;
        b2BodyDef bodyDef;
        b2Body* body;
        b2Fixture* fixture;
        b2Shape* shape;
        IPhysicsListener* listener;

        void setup(srend::GameNode*, IPhysicsListener*);
        void setup(srend::GameNode*, IPhysicsListener*, b2BodyType);
        void makeRectShape(float w, float h);

        void updateBodyTransform();
        void updateNodeTransform();

    private:

        int changeEnabled = -1;

        void _onNodeEnableChanged(bool e);
        void _updateState();

        void _onOwnerDisabled();
        void _onOwnerEnabled();
    };

    struct FontChar {
        unsigned int textureID;  // ID handle of the glyph texture
        int2   size;       // Size of glyph
        int2   bearing;    // Offset from baseline to left/top of glyph
        long advance;    // Offset to advance to next glyph
    };

    struct Font {
        FT_Face face;

        void setSize(int sz);
        FontChar* character(char c);

        void release();
    private:
        int _getHash(int,char);
        std::unordered_map<int, FontChar> _charCache;
        int _lastSize;
    };

    // struct TextDrawInfo {

    // }
}