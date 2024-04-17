///
/// @file NodeConnectionInteraction.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef NODE_CONNECTION_INTERACTION_HXX_4A707E321945
#define NODE_CONNECTION_INTERACTION_HXX_4A707E321945

#include <memory>
#include <QtCore/QPointF>

#include "Definitions.hxx"

namespace QtNodes {

class ConnectionGraphicsObject;
class NodeGraphicsObject;
class BasicGraphicsScene;

///
/// @brief Class wraps conecting and disconnecting checks. An instance should be
/// created on the stack and destroyed automatically when the operation is
/// completed.
///
class NodeConnectionInteraction
{
public:
    NodeConnectionInteraction(
        NodeGraphicsObject&       ngo,
        ConnectionGraphicsObject& cgo,
        BasicGraphicsScene&       scene
    );

    /**
     * Can connect when following conditions are met:
     * 1. Connection 'requires' a port.
     * 2. Connection loose end is above the node port.
     * 3. Source and target `nodeId`s are different.
     * 4. GraphModel permits connection.
     */
    bool canConnect(PortIndex* portIndex) const;

    /// Creates a new connectino if possible.
    /**
     * 1. Check conditions from 'canConnect'.
     * 2. Creates new connection with `GraphModel::addConnection`.
     * 3. Adjust connection geometry.
     */
    bool tryConnect() const;

    /**
     * 1. Delete connection with `GraphModel::deleteConnection`.
     * 2. Create a "draft" connection with incomplete `ConnectionId`.
     * 3. Repaint both previously connected nodes.
     */
    bool disconnect(PortType portToDisconnect) const;

private:
    PortType connectionRequiredPort() const;

    QPointF connectionEndScenePosition(PortType) const;

    QPointF nodePortScenePosition(PortType portType, PortIndex portIndex) const;

    PortIndex nodePortIndexUnderScenePoint(PortType portType, const QPointF& p)
        const;

private:
    NodeGraphicsObject&       _ngo;
    ConnectionGraphicsObject& _cgo;
    BasicGraphicsScene&       _scene;
};

}  // namespace QtNodes

#endif  // NODE_CONNECTION_INTERACTION_HXX_4A707E321945