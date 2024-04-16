///
/// @file ConnectionState.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef CONNECTION_STATE_HXX_CE7905BE6ED1
#define CONNECTION_STATE_HXX_CE7905BE6ED1

#include <QtCore/QUuid>

#include "Definitions.hxx"
#include "Export.hxx"

class QPointF;

namespace QtNodes {

class ConnectionGraphicsObject;

///
/// @brief Stores currently draggind end. Remembers last hovered Node.
///
class NODE_EDITOR_PUBLIC ConnectionState
{
public:
    ///
    /// @brief Defines whether we construct a new connection or it is already
    /// binding two nodes.
    ///
    enum LooseEnd
    {
        Pending   = 0,
        Connected = 1
    };

public:
    ConnectionState(ConnectionGraphicsObject& cgo)
        : _cgo(cgo)
        , _hovered(false)
    {
    }

    ConnectionState(const ConnectionState&) = delete;
    ConnectionState(ConnectionState&&)      = delete;

    ConnectionState& operator=(const ConnectionState&) = delete;
    ConnectionState& operator=(ConnectionState&&)      = delete;

    ~ConnectionState();

public:
    PortType requiredPort() const;
    bool     requiresPort() const;

    bool hovered() const;
    void setHovered(bool hovered);

public:
    ///
    /// @brief Caches NodeId for further interaction.
    ///
    /// @param nodeId
    ///
    void setLastHoveredNode(const NodeId nodeId);

    NodeId lastHoveredNode() const;

    void resetLastHoveredNode();

private:
    ConnectionGraphicsObject& _cgo;

    bool _hovered;

    NodeId _lastHoveredNode { InvalidNodeId };
};

}  // namespace QtNodes

#endif  // CONNECTION_STATE_HXX_CE7905BE6ED1
