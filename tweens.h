#pragma once

#include <functional>
#include "tween_core.h"
#include "types.h"

namespace srend {

    namespace Tween {
        class Value : public TweenBase {

        public:
            static Value* create(float from, float to, float t, std::function<void(float)> onStep);
            std::function<void(float)> step;
        protected:
            void update() override;
        private:
            float _from;
            float _to;
        };

        static TweenBase* Scale(Transform& t, float to, float time){
    
            float from = t.scale.x < t.scale.y ? t.scale.x : t.scale.y;

            auto tw = Value::create(from, to, time, [&t](float x) {
                t.scale.x = x;
                t.scale.y = x;
            });
            
            return(TweenBase*)tw;
        }
    };
};