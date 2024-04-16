///
/// @file NodeState.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef NODE_STATE_HXX_A1E4BF588D58
#define NODE_STATE_HXX_A1E4BF588D58

#include <QtCore/QPointer>
#include <QtCore/QPointF>
#include <QtCore/QUuid>
#include <unordered_map>
#include <vector>

#include "Definitions.hxx"
#include "Export.hxx"
#include "NodeData.hxx"

namespace QtNodes {

class ConnectionGraphicsObject;
class NodeGraphicsObject;

///
/// @brief Stores bool for hovering connections and resizing flag.
///
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

    ///
    /// @brief QPointer tracks the QObject inside and is automatically cleared
    /// when the object is destroyed.
    ///
    QPointer<const ConnectionGraphicsObject> _connectionForReaction;
};

}  // namespace QtNodes

#endif  // NODE_STATE_HXX_A1E4BF588D58
