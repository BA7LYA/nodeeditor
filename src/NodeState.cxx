///
/// @file NodeState.cxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#include "NodeState.hxx"

#include "ConnectionGraphicsObject.hxx"
#include "NodeGraphicsObject.hxx"

namespace QtNodes {

NodeState::NodeState(NodeGraphicsObject& ngo)
    : _ngo(ngo)
    , _hovered(false)
    , _resizing(false)
    , _connectionForReaction { nullptr }
{
    Q_UNUSED(_ngo);
}

void NodeState::setResizing(bool resizing)
{
    _resizing = resizing;
}

bool NodeState::resizing() const
{
    return _resizing;
}

const ConnectionGraphicsObject* NodeState::connectionForReaction() const
{
    return _connectionForReaction.data();
}

void NodeState::storeConnectionForReaction(const ConnectionGraphicsObject* cgo)
{
    _connectionForReaction = cgo;
}

void NodeState::resetConnectionForReaction()
{
    _connectionForReaction.clear();
}

}  // namespace QtNodes
