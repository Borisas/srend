#include "scene.h"
#include "core.h"
srend::Scene::Scene(){}
srend::Scene::~Scene(){}

void srend::Scene::load() {

}

void srend::Scene::unload() {
    
}

void srend::Scene::update() {

    _destroyQueuedNodes();

    traverseNodeTree(&_root, [](GameNode* n) {
        n->resolveFrameStart();
    });

    traverseNodeTree(&_root, [](GameNode* n){ 
        if ( !n->isEnabledInHierarchy() ) return;
        n->update(); 
    });
}

void srend::Scene::lateUpdate() {

    traverseNodeTree(&_root, [](GameNode* n){
        if ( !n->isEnabledInHierarchy() ) return;
        n->lateUpdate(); 
    });
}

void srend::Scene::draw() {

    srend::traverseNodeTree(&_root, [](GameNode* n){ 
        if ( !n->isEnabledInHierarchy() ) return;
        n->draw(); 
    });
}

void srend::Scene::frameCompleted() {
}

srend::GameNode* srend::Scene::getRoot() {
    return &_root;
}

srend::GameNode* srend::Scene::assignNode(std::unique_ptr<GameNode> & n) {
    _allocatedNodes.push_back(std::move(n));
    _allocatedNodes[_allocatedNodes.size()-1]->scene = this;
    auto ret = _allocatedNodes[_allocatedNodes.size()-1].get();
    ret->setParent(&_root);
    return ret;
}

srend::GameNode* srend::Scene::assignNode(GameNode* n) {
    _allocatedNodes.push_back(std::unique_ptr<GameNode>(n));
    _allocatedNodes[_allocatedNodes.size()-1]->scene = this;
    n->setParent(&_root);
    auto ret = _allocatedNodes[_allocatedNodes.size()-1].get();
    ret->setParent(&_root);
    return ret;
}

void srend::Scene::setNodeDestroyed() {
    _nodeDestroyed = true;
}

srend::GameNode* srend::Scene::findNode(const char* name) {
    
    srend::GameNode* ret = nullptr;
    
    traverseNodeTree(&_root, [&ret, name](GameNode* n){
        if ( n->name == name ) {
            ret = n;
        }
    });

    return ret;
}

void srend::Scene::_destroyQueuedNodes() {

    if ( !_nodeDestroyed ) return;

    for ( int i = 0; i < _allocatedNodes.size(); i++ ) {
        if ( !_allocatedNodes[i]->isSetToDestroy() ) continue;

        auto & ptr = _allocatedNodes[i];
        ptr->setParent(nullptr);

        _allocatedNodes.erase(_allocatedNodes.begin()+i);
        i--;

    }

    _nodeDestroyed = false;
}