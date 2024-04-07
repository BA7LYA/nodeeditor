#include "QtNodes/ConnectionState.hxx"

#include <QtCore/QDebug>
#include <QtCore/QPointF>

#include "QtNodes/BasicGraphicsScene.hxx"
#include "QtNodes/ConnectionGraphicsObject.hxx"
#include "QtNodes/NodeGraphicsObject.hxx"

namespace QtNodes {

ConnectionState::~ConnectionState()
{
    // resetLastHoveredNode();
}

PortType ConnectionState::requiredPort() const
{
    PortType t = PortType::None;

    if (_cgo.connectionId().outNodeId == InvalidNodeId)
    {
        t = PortType::Out;
    }
    else if (_cgo.connectionId().inNodeId == InvalidNodeId)
    {
        t = PortType::In;
    }

    return t;
}

bool ConnectionState::requiresPort() const
{
    const ConnectionId& id = _cgo.connectionId();
    return id.outNodeId == InvalidNodeId || id.inNodeId == InvalidNodeId;
}

bool ConnectionState::hovered() const
{
    return _hovered;
}

void ConnectionState::setHovered(bool hovered)
{
    _hovered = hovered;
}

void ConnectionState::setLastHoveredNode(const NodeId nodeId)
{
    _lastHoveredNode = nodeId;
}

NodeId ConnectionState::lastHoveredNode() const
{
    return _lastHoveredNode;
}

void ConnectionState::resetLastHoveredNode()
{
    if (_lastHoveredNode != InvalidNodeId)
    {
        auto ngo = _cgo.nodeScene()->nodeGraphicsObject(_lastHoveredNode);
        ngo->update();
    }

    _lastHoveredNode = InvalidNodeId;
}

}  // namespace QtNodes
