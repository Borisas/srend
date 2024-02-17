#include "game_node.h"
#include "core.h"

srend::GameNode::GameNode() { 

    transform.setOwner(this);
    transform.onUpdated = [&]() { _dirtyAllChildren(); };
}
srend::GameNode::~GameNode() { }

void srend::GameNode::setup() {}

srend::GameNode* srend::GameNode::create() {
    if(srend::_scene == nullptr) 
        return nullptr;
    auto ret = (GameNode*)srend::_scene->assignNode(new GameNode());
    ret->setup();
    return ret;
}

srend::GameNode* srend::GameNode::getParent() {
    return _parent;
}

void srend::GameNode::setParent(GameNode* other) {

    if ( _parent != nullptr ) {
        auto pp = _parent;
        
        int rm = -1;
        for ( int i = 0; i < pp->_children.size(); i++ ) {
            if ( pp->_children[i] == this) {
                rm = i;
                break;
            }
        }
        if ( rm != -1 ) {
            _parent = nullptr;
            pp->_children.erase(pp->_children.begin() + rm);
        }
    }
    _parent = other;

    if ( other != nullptr ) {
        other->_children.push_back(this);
        _dirtyAllChildren();
    }
}

int srend::GameNode::getChildCount() {
    return _children.size();
}

srend::GameNode* srend::GameNode::getChild(int i) {
    if ( _children.size() <= i || i < 0 ) {
        return nullptr;
    }
    return _children[i];
}

void srend::GameNode::update() {

}

void srend::GameNode::lateUpdate() {

}

void srend::GameNode::draw() {
    
}

void srend::GameNode::resolveFrameStart() {

    if ( _enableChange != -1 ) {
        
        if ( _enableChange == 0 ) {
            _enabled = false;
            onDisable.invoke();
        }
        else if ( _enableChange == 1 ) {
            _enabled = true;
            onEnable.invoke();
        }
        _enableChange = -1;
    }
}

void srend::GameNode::destroy() {
    if ( _setToDestroy ) return;

    _scene->setNodeDestroyed();
    _setToDestroy = true;

    for ( int i = 0; i < _children.size(); i++ ) {
        _children[i]->destroy();
    }
}

bool srend::GameNode::isSetToDestroy() {
    return _setToDestroy;
}

bool srend::GameNode::isEnabled() {
    if ( _enableChange != -1 ) {
        return _enableChange == 1 ? true : false;
    }
    return _enabled;
}

bool srend::GameNode::isEnabledInHierarchy() {
    if ( !isEnabled() ) return false;
    if ( _parent != nullptr ) {
        return _parent->isEnabledInHierarchy();
    }
    return true;
}

void srend::GameNode::setEnabled(bool e) {
    if ( e == _enabled && _enableChange == -1 ) return;
    _enableChange = e ? 1 : 0;
}

void srend::GameNode::_dirtyAllChildren() {
    // srend::Scene::traverseTree();
    srend::traverseNodeTree(this,[this](GameNode* n) {
        if ( n == this ) return;//ignore self
        n->transform.forceDirty();
    });
}