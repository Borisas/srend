#pragma once

#include "game_node.h"
#include "types.h"
#include <string>

namespace srend {

    enum struct AnimationStopAction { 
        NONE = 0,
        DISABLE = 1,
        DESTROY = 2,
    };

    class GameAnimation : public GameNode {

    public:
        static GameAnimation* create();

        void setup() override;

        void loadAnimation(std::string, int, int);
        void loadProgramDefault();
        void loadProgram(std::string, std::string);
        void setFrameRate(int);
        void setOverrideTotalFrames(int);
        void setLoops(int);
        void setStopAction(AnimationStopAction);

        void draw() override;
        void update() override;

        srend::Event<void> onFinished;

    protected:

        void _onSelfEnable();

        int _totalFrames();
        float4 _calculateFrameUv();
        void _performStopAction();

        Sprite _sprite;
        int _overrideTotalFrames;
        int _calculatedTotalFrames;
        int _frameWidth;
        int _frameHeight;
        int _frameRate;
        int _frameIndex;

        float _animTimer = 0.0f;
        int _loops = -1;
        int _loopsCompleted = 0;

        AnimationStopAction _stopAction = AnimationStopAction::NONE;
    };

};
