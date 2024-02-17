#pragma once

#include <vector>
#include <functional>
#include <memory>
#include "types.h"
#include "game_node.h"

namespace srend{
    
    class Scene {
    public:

        Scene();
        ~Scene();

        virtual void load();
        virtual void unload();

        virtual void update();
        virtual void lateUpdate();
        virtual void draw();

        void frameCompleted();

        GameNode* getRoot();

        GameNode* assignNode(std::unique_ptr<GameNode> &);
        GameNode* assignNode(GameNode*);

        void setNodeDestroyed();

        GameNode* findNode(const char* name);
    private:

        void _destroyQueuedNodes();

        GameNode _root;
        std::vector<std::unique_ptr<GameNode> > _allocatedNodes;
        bool _nodeDestroyed = false;
    };
}