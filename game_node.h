#pragma once

#include "types.h"
#include "event.h"

namespace srend  {

    class Scene;

    class GameNode { 

    public:

        static GameNode* create();

        GameNode();
        virtual ~GameNode();

        virtual void setup();

        GameNode* getParent();
        void setParent(GameNode* other);
        int getChildCount();
        GameNode* getChild(int index);

        virtual void update();
        virtual void lateUpdate();
        virtual void draw();
        void resolveFrameStart();

        Scene* scene;
        srend::Transform transform;

        void destroy();
        bool isSetToDestroy();
        bool isEnabled();
        bool isEnabledInHierarchy();
        void setEnabled(bool e);

        srend::Event<void> onEnable;
        srend::Event<void> onDisable;

        std::string name;
        
    private:
        void _dirtyAllChildren();
    protected:
        bool _enabled = true;
        GameNode* _parent;
        std::vector<GameNode*> _children;
        bool _setToDestroy = false;

        int _enableChange = -1;
    };
}