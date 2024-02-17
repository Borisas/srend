#pragma once

#include "game_node.h"

namespace srend {
    class GameText : public GameNode {

    public:
        static GameText* create();

        void setup() override;

        void loadFont(std::string);
        void loadProgramDefault();
        void loadProgram(std::string, std::string);

        void draw() override;

        void setText(const std::string& text);
        void setSize(int);

        void setColor(const srend::float4&);
    
        VerticalAlignment verticalAlignment = VerticalAlignment::CENTER;
        HorizontalAlignment horizontalAlignment = HorizontalAlignment::CENTER;

        float getWidth();
        float getHeight();

    private:

        srend::float4 _color;
        std::string _text;
        srend::Font* _font;
        GLProgram* _program;
        int _localSize = 18;

        void _calculateSizeCache();
        bool _sizeCacheDirty = true;
        float _cachedWidth;
        float _cachedHeight;
    };
}