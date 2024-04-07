#pragma once

#include <QtCore/QJsonObject>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <unordered_map>
#include <unordered_set>

#include "QtNodes/ConnectionIdHash.hxx"
#include "QtNodes/Definitions.hxx"
#include "QtNodes/Export.hxx"

namespace QtNodes {

/**
 * The central class in the Model-View approach. It delivers all kinds
 * of information from the backing user data structures that represent
 * the graph. The class allows to modify the graph structure: create
 * and remove nodes and connections.
 *
 * We use two types of the unique ids for graph manipulations:
 *   - NodeId
 *   - ConnectionId
 */
class NODE_EDITOR_PUBLIC AbstractGraphModel : public QObject
{
    Q_OBJECT

public:
    /// Generates a new unique NodeId.
    virtual NodeId newNodeId() = 0;

    /// @brief Returns the full set of unique Node Ids.
    /**
     * Model creator is responsible for generating unique `unsigned int`
     * Ids for all the nodes in the graph. From an Id it should be
     * possible to trace back to the model's internal representation of
     * the node.
     */
    virtual std::unordered_set<NodeId> allNodeIds() const = 0;

    /**
     * A collection of all input and output connections for the given `nodeId`.
     */
    virtual std::unordered_set<ConnectionId> allConnectionIds(
        const NodeId nodeId
    ) const
        = 0;

    /// @brief Returns all connected Node Ids for given port.
    /**
     * The returned set of nodes and port indices correspond to the type
     * opposite to the given `portType`.
     */
    virtual std::unordered_set<ConnectionId> connections(
        NodeId    nodeId,
        PortType  portType,
        PortIndex index
    ) const
        = 0;

    /// Checks if two nodes with the given `connectionId` are connected.
    virtual bool connectionExists(const ConnectionId connectionId) const = 0;

    /// Creates a new node instance in the derived class.
    /**
     * The model is responsible for generating a unique `NodeId`.
     * @param[in] nodeType is free to be used and interpreted by the
     * model on its own, it helps to distinguish between possible node
     * types and create a correct instance inside.
     */
    virtual NodeId addNode(const QString nodeType = QString()) = 0;

    /// Model decides if a conection with a given connection Id possible.
    /**
     * The default implementation compares corresponding data types.
     *
     * It is possible to override the function and connect non-equal
     * data types.
     */
    virtual bool connectionPossible(const ConnectionId connectionId) const = 0;

    /// Defines if detaching the connection is possible.
    virtual bool detachPossible(const ConnectionId) const
    {
        return true;
    }

    /// Creates a new connection between two nodes.
    /**
     * Default implementation emits signal
     * `connectionCreated(connectionId)`
     *
     * In the derived classes user must emite the signal to notify the
     * scene about the changes.
     */
    virtual void addConnection(const ConnectionId connectionId) = 0;

    /**
     * @returns `true` if there is data in the model associated with the
     * given `nodeId`.
     */
    virtual bool nodeExists(const NodeId nodeId) const = 0;

    /// @brief Returns node-related data for requested NodeRole.
    /**
     * @returns Node Caption, Node Caption Visibility, Node Position etc.
     */
    virtual QVariant nodeData(NodeId nodeId, NodeRole role) const = 0;

    /**
     * A utility function that unwraps the `QVariant` value returned from the
     * standard `QVariant AbstractGraphModel::nodeData(NodeId, NodeRole)`
     * function.
     */
    template<typename T>
    T nodeData(NodeId nodeId, NodeRole role) const
    {
        return nodeData(nodeId, role).value<T>();
    }

    virtual NodeFlags nodeFlags(NodeId nodeId) const
    {
        Q_UNUSED(nodeId);
        return NodeFlag::NoFlags;
    }

    /// @brief Sets node properties.
    /**
     * Sets: Node Caption, Node Caption Visibility,
     * Shyle, State, Node Position etc.
     * @see NodeRole.
     */
    virtual bool setNodeData(NodeId nodeId, NodeRole role, QVariant value) = 0;

    /// @brief Returns port-related data for requested NodeRole.
    /**
     * @returns Port Data Type, Port Data, Connection Policy, Port
     * Caption.
     */
    virtual QVariant portData(
        NodeId    nodeId,
        PortType  portType,
        PortIndex index,
        PortRole  role
    ) const
        = 0;

    /**
     * A utility function that unwraps the `QVariant` value returned from the
     * standard `QVariant AbstractGraphModel::portData(...)` function.
     */
    template<typename T>
    T portData(NodeId nodeId, PortType portType, PortIndex index, PortRole role)
        const
    {
        return portData(nodeId, portType, index, role).value<T>();
    }

    virtual bool setPortData(
        NodeId          nodeId,
        PortType        portType,
        PortIndex       index,
        const QVariant& value,
        PortRole        role = PortRole::Data
    ) = 0;

    virtual bool deleteConnection(const ConnectionId connectionId) = 0;

    virtual bool deleteNode(const NodeId nodeId) = 0;

    /**
     * Reimplement the function if you want to store/restore the node's
     * inner state during undo/redo node deletion operations.
     */
    virtual QJsonObject saveNode(const NodeId) const
    {
        return {};
    }

    /**
     * Reimplement the function if you want to support:
     *
     *   - graph save/restore operations,
     *   - undo/redo operations after deleting the node.
     *
     * QJsonObject must contain following fields:
     *
     *
     * ```
     * {
     *   id : 5,
     *   position : { x : 100, y : 200 },
     *   internal-data {
     *     "your model specific data here"
     *   }
     * }
     * ```
     *
     * The function must do almost exacly the same thing as the normal
     * addNode(). The main difference is in a model-specific `inner-data`
     * processing.
     */
    virtual void loadNode(const QJsonObject&) {}

public:
    /**
     * Function clears connections attached to the ports that are scheduled to
     * be deleted. It must be called right before the model removes its old port
     * data.
     *
     * @param nodeId Defines the node to be modified
     * @param portType Is either PortType::In or PortType::Out
     * @param first Index of the first port to be removed
     * @param last Index of the last port to be removed
     */
    void portsAboutToBeDeleted(
        const NodeId    nodeId,
        const PortType  portType,
        const PortIndex first,
        const PortIndex last
    );

    /**
     * Signal emitted when model no longer has the old data associated with the
     * given port indices and when the node must be repainted.
     */
    void portsDeleted();

    /**
     * Signal emitted when model is about to create new ports on the given node.
     * @param first Is the first index of the new port after insertion.
     * @param last Is the last index of the new port after insertion.
     *
     * Function caches existing connections that are located after the `last`
     * port index. For such connections the new "post-insertion" addresses are
     * computed and stored until the function AbstractGraphModel::portsInserted
     * is called.
     */
    void portsAboutToBeInserted(
        const NodeId    nodeId,
        const PortType  portType,
        const PortIndex first,
        const PortIndex last
    );

    /**
     * Function re-creates the connections that were shifted during the port
     * insertion. After that the node is updated.
     */
    void portsInserted();

Q_SIGNALS:
    void connectionCreated(const ConnectionId connectionId);

    void connectionDeleted(const ConnectionId connectionId);

    void nodeCreated(const NodeId nodeId);

    void nodeDeleted(const NodeId nodeId);

    void nodeUpdated(const NodeId nodeId);

    void nodeFlagsUpdated(const NodeId nodeId);

    void nodePositionUpdated(const NodeId nodeId);

    void modelReset();

private:
    std::vector<ConnectionId> _shiftedByDynamicPortsConnections;
};

}  // namespace QtNodes
