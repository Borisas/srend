#pragma once

#include <vector>
#include <memory>
#include "event.h"

namespace srend {

    class TweenBase {
    public:

        static bool IsValid(TweenBase* ptr);

        TweenBase();
        virtual ~TweenBase();

        virtual bool isCompleted();
        virtual bool isKilled();
        virtual void kill();
        srend::Event<void> onCompleted;
    protected:
        float getT();
        virtual void update() = 0;    
        float _time;
        float _duration;
    private:
        bool _killed;
    
    friend class TweenCore;
    };

    class TweenCore {
    public:

        void update();
        srend::TweenBase* assign(TweenBase* tween);
        srend::TweenBase* assign(std::unique_ptr<TweenBase> tween);
        bool isValid(TweenBase* ptr);

    private:
        std::vector<std::unique_ptr<TweenBase> > _tweens;
    };
}