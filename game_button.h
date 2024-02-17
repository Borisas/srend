#pragma once

#include "game_sprite.h"

namespace srend {
    class GameButton : public GameSprite {

    public:
        void update() override;
        void draw() override;

        virtual void onHoverEnter();
        virtual void onHoverExit();
        virtual void onClick();
        virtual void onRelease();
        virtual void onContext();

        void overrideWidth(int w);
        void overrideHeight(int h);
        int width();
        int height();

        srend::float2 getSize() override;

        void setDrawn(bool d);
    
    private:

        bool _isMouseOver(const float2& mpos);
        bool _clicked = false;
        bool _drawn = true;

    protected:
        int _width = -1;
        int _height = -1;
        bool _hovered = false;

    };
}