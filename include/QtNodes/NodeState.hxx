#pragma once

#include <QtCore/QPointer>
#include <QtCore/QPointF>
#include <QtCore/QUuid>
#include <unordered_map>
#include <vector>

#include "QtNodes/Definitions.hxx"
#include "QtNodes/Export.hxx"
#include "QtNodes/NodeData.hxx"

namespace QtNodes {

class ConnectionGraphicsObject;
class NodeGraphicsObject;

/// Stores bool for hovering connections and resizing flag.
class NODE_EDITOR_PUBLIC NodeState
{
public:
    NodeState(NodeGraphicsObject& ngo);

public:
    bool hovered() const
    {
        return _hovered;
    }

    void setHovered(bool hovered = true)
    {
        _hovered = hovered;
    }

    void setResizing(bool resizing);

    bool resizing() const;

    const ConnectionGraphicsObject* connectionForReaction() const;

    void storeConnectionForReaction(const ConnectionGraphicsObject* cgo);

    void resetConnectionForReaction();

private:
    NodeGraphicsObject& _ngo;

    bool _hovered;

    bool _resizing;

    // QPointer tracks the QObject inside and is automatically cleared
    // when the object is destroyed.
    QPointer<const ConnectionGraphicsObject> _connectionForReaction;
};

}  // namespace QtNodes
