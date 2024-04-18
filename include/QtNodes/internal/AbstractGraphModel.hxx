///
/// @file AbstractGraphModel.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-16
/// @copyright Copyright (c) 2024
///

#ifndef ABSTRACT_GRAPH_MODEL_HXX_49E67789F271
#define ABSTRACT_GRAPH_MODEL_HXX_49E67789F271

#include <QtCore/QJsonObject>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <unordered_map>
#include <unordered_set>

#include "ConnectionIdHash.hxx"
#include "Definitions.hxx"
#include "Export.hxx"

namespace QtNodes {

///
/// @brief The central class in the Model-View approach. It delivers all kinds
/// of information from the backing user data structures that represent the
/// graph. The class allows to modify the graph structure: create and remove
/// nodes and connections.
///
/// We use two types of the unique ids for graph manipulations:
///  - NodeId
///  - ConnectionId
///
class NODE_EDITOR_PUBLIC AbstractGraphModel : public QObject
{
    Q_OBJECT

public:
    ///
    /// TODO(ba7lya): Add a class to generate new unique NodeId.
    ///

    ///
    /// @brief Generates a new unique NodeId.
    /// @return
    ///
    virtual NodeId newNodeId() = 0;

    ///
    /// @brief Returns the full set of unique Node Ids. Model creator is
    /// responsible for generating unique `unsigned int` Ids for all the nodes
    /// in the graph. From an Id it should be possible to trace back to the
    /// model's internal representation of the node.
    ///
    virtual std::unordered_set<NodeId> allNodeIds() const = 0;

    ///
    /// @brief A collection of all input and output connections for the given
    /// `nodeId`.
    /// @param nodeId
    /// @return std::unordered_set<ConnectionId>
    ///
    virtual std::unordered_set<ConnectionId> allConnectionIds(
        const NodeId nodeId
    ) const
        = 0;

    ///
    /// @brief Returns all connected Node Ids for given port.
    /// @param nodeId
    /// @param portType
    /// @param index
    /// @return The returned set of nodes and port indices correspond to the
    /// type opposite to the given `portType`.
    ///
    virtual std::unordered_set<ConnectionId> connections(
        NodeId    nodeId,
        PortType  portType,
        PortIndex index
    ) const
        = 0;

    ///
    /// @brief Checks if two nodes with the given `connectionId` are connected.
    ///
    /// @param connectionId
    /// @return true
    /// @return false
    ///
    virtual bool connectionExists(const ConnectionId connectionId) const = 0;

    ///
    /// @brief Creates a new node instance in the derived class.
    /// @param nodeType It is free to be used and interpreted by the model on
    /// its own, it helps to distinguish between possible node types and create
    /// a correct instance inside.
    /// @return `NodeId` - The model is responsible for generating a unique
    /// `NodeId`.
    ///
    virtual NodeId addNode(const QString nodeType = QString()) = 0;

    ///
    /// @brief Model decides if a conection with a given connection Id possible.
    /// The default implementation compares corresponding data types. It is
    /// possible to override the function and connect non-equal data types.
    /// @param connectionId
    /// @return
    ///
    virtual bool connectionPossible(const ConnectionId connectionId) const = 0;

    ///
    /// @brief Defines if detaching the connection is possible.
    ///
    /// @return true
    /// @return false
    ///
    virtual bool detachPossible(const ConnectionId) const
    {
        return true;
    }

    ///
    /// @brief Creates a new connection between two nodes. Default
    /// implementation emits signal `connectionCreated(connectionId)`. In the
    /// derived classes user must emite the signal to notify the scene about the
    /// changes.
    /// @param connectionId
    ///
    virtual void addConnection(const ConnectionId connectionId) = 0;

    ///
    /// @brief
    /// @param nodeId
    /// @return `true` if there is data in the model associated with the given
    /// `nodeId`.
    ///
    virtual bool nodeExists(const NodeId nodeId) const = 0;

    ///
    /// @brief Node Caption, Node Caption Visibility, Node Position etc.
    /// @param nodeId
    /// @param role
    /// @return Returns node-related data for requested NodeRole.
    ///
    virtual QVariant nodeData(NodeId nodeId, NodeRole role) const = 0;

    ///
    /// @brief A utility function that unwraps the `QVariant` value returned
    /// from the standard `QVariant AbstractGraphModel::nodeData(NodeId,
    /// NodeRole)` function.
    /// @tparam T
    /// @param nodeId
    /// @param role
    /// @return
    ///
    template<typename T>
    T nodeData(NodeId nodeId, NodeRole role) const
    {
        return nodeData(nodeId, role).value<T>();
    }

    ///
    /// @brief
    /// @param nodeId
    /// @return
    ///
    virtual NodeFlags nodeFlags(NodeId nodeId) const
    {
        Q_UNUSED(nodeId);
        return NodeFlag::NoFlags;
    }

    ///
    /// @brief Sets node properties. Sets: Node Caption, Node Caption
    /// Visibility, Shyle, State, Node Position etc.
    /// @see NodeRole.
    /// @param nodeId
    /// @param role
    /// @param value
    /// @return
    ///
    virtual bool setNodeData(NodeId nodeId, NodeRole role, QVariant value) = 0;

    ///
    /// @brief Returns port-related data for requested NodeRole.
    /// @param nodeId
    /// @param portType
    /// @param index
    /// @param role
    /// @return Port Data Type, Port Data, Connection Policy, Port Caption.
    ///
    virtual QVariant portData(
        NodeId    nodeId,
        PortType  portType,
        PortIndex index,
        PortRole  role
    ) const
        = 0;

    ///
    /// @brief A utility function that unwraps the `QVariant` value returned
    /// from the standard `QVariant AbstractGraphModel::portData(...)` function.
    /// @tparam T
    /// @param nodeId
    /// @param portType
    /// @param index
    /// @param role
    /// @return
    ///
    template<typename T>
    T portData(NodeId nodeId, PortType portType, PortIndex index, PortRole role)
        const
    {
        return portData(nodeId, portType, index, role).value<T>();
    }

    ///
    /// @brief
    /// @param nodeId
    /// @param portType
    /// @param index
    /// @param value
    /// @param role
    /// @return
    ///
    virtual bool setPortData(
        NodeId          nodeId,
        PortType        portType,
        PortIndex       index,
        const QVariant& value,
        PortRole        role = PortRole::Data
    ) = 0;

    ///
    /// @brief
    /// @param connectionId
    /// @return
    ///
    virtual bool deleteConnection(const ConnectionId connectionId) = 0;

    ///
    /// @brief
    /// @param nodeId
    /// @return
    ///
    virtual bool deleteNode(const NodeId nodeId) = 0;

    ///
    /// @brief Reimplement the function if you want to store/restore the node's
    /// inner state during undo/redo node deletion operations.
    /// @param
    /// @return
    ///
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
    ///
    /// @brief Function clears connections attached to the ports that are
    /// scheduled to be deleted. It must be called right before the model
    /// removes its old port data.
    /// @param nodeId Defines the node to be modified
    /// @param portType Is either PortType::In or PortType::Out
    /// @param first Index of the first port to be removed
    /// @param last Index of the last port to be removed
    ///
    void portsAboutToBeDeleted(
        const NodeId    nodeId,
        const PortType  portType,
        const PortIndex first,
        const PortIndex last
    );

    ///
    /// @brief Signal emitted when model no longer has the old data associated
    /// with the given port indices and when the node must be repainted.
    ///
    void portsDeleted();

    ///
    /// @brief Signal emitted when model is about to create new ports on the
    /// given node. Function caches existing connections that are located after
    /// the `last` port index. For such connections the new "post-insertion"
    /// addresses are computed and stored until the function
    /// AbstractGraphModel::portsInserted is called.
    /// @param nodeId
    /// @param portType
    /// @param first Is the first index of the new port after insertion.
    /// @param last Is the last index of the new port after insertion.
    ///
    void portsAboutToBeInserted(
        const NodeId    nodeId,
        const PortType  portType,
        const PortIndex first,
        const PortIndex last
    );

    ///
    /// @brief Function re-creates the connections that were shifted during the
    /// port insertion. After that the node is updated.
    ///
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

#endif  // ABSTRACT_GRAPH_MODEL_HXX_49E67789F271
