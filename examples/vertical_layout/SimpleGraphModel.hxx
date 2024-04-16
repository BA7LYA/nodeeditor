///
/// @file SimpleGraphModel.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef SIMPLE_GRAPH_MODEL_HXX_7C4751E355BF
#define SIMPLE_GRAPH_MODEL_HXX_7C4751E355BF

#include <QtCore/QJsonObject>
#include <QtCore/QPointF>
#include <QtCore/QSize>
#include <QtNodes/AbstractGraphModel>
#include <QtNodes/ConnectionIdUtils>
#include <QtNodes/StyleCollection>
#include <unordered_map>
#include <unordered_set>

using ConnectionId     = QtNodes::ConnectionId;
using ConnectionPolicy = QtNodes::ConnectionPolicy;
using NodeFlag         = QtNodes::NodeFlag;
using NodeId           = QtNodes::NodeId;
using NodeRole         = QtNodes::NodeRole;
using PortIndex        = QtNodes::PortIndex;
using PortRole         = QtNodes::PortRole;
using PortType         = QtNodes::PortType;
using StyleCollection  = QtNodes::StyleCollection;

using QtNodes::InvalidNodeId;

///
/// @brief The class implements a bare minimum required to demonstrate a
/// model-based graph.
///
class SimpleGraphModel : public QtNodes::AbstractGraphModel
{
    Q_OBJECT

public:
    struct NodeGeometryData
    {
        QSize   size;
        QPointF pos;
    };

    SimpleGraphModel();

    ~SimpleGraphModel() override;

    std::unordered_set<NodeId> allNodeIds() const override;

    std::unordered_set<ConnectionId> allConnectionIds(const NodeId nodeId
    ) const override;

    std::unordered_set<ConnectionId> connections(
        NodeId    nodeId,
        PortType  portType,
        PortIndex portIndex
    ) const override;

    bool connectionExists(const ConnectionId connectionId) const override;

    NodeId addNode(const QString nodeType = QString()) override;

    ///
    /// @brief Connection is possible when graph contains no connectivity data
    /// in both directions `Out -> In` and `In -> Out`.
    /// @param connectionId
    /// @return
    ///
    bool connectionPossible(const ConnectionId connectionId) const override;

    void addConnection(const ConnectionId connectionId) override;

    bool nodeExists(const NodeId nodeId) const override;

    QVariant nodeData(NodeId nodeId, NodeRole role) const override;

    bool setNodeData(NodeId nodeId, NodeRole role, QVariant value) override;

    QVariant portData(
        NodeId    nodeId,
        PortType  portType,
        PortIndex portIndex,
        PortRole  role
    ) const override;

    bool setPortData(
        NodeId          nodeId,
        PortType        portType,
        PortIndex       portIndex,
        const QVariant& value,
        PortRole        role = PortRole::Data
    ) override;

    bool deleteConnection(const ConnectionId connectionId) override;

    bool deleteNode(const NodeId nodeId) override;

    QJsonObject saveNode(const NodeId) const override;

    ///
    /// @brief Creates a new node based on the informatoin in `nodeJson`.
    /// @param nodeJson conains a `NodeId`, node's position, internal node
    /// information.
    ///
    void loadNode(const QJsonObject& nodeJson) override;

private:
    NodeId newNodeId() override
    {
        return _nextNodeId++;
    }

private:
    std::unordered_set<NodeId> _nodeIds;

    ///
    /// @brief [Important] This is a user defined data structure backing your
    /// model. In your case it could be anything else representing a graph, for
    /// example, a table. Or a collection of structs with pointers to each
    /// other. Or an abstract syntax tree, you name it. This data structure
    /// contains the graph connectivity information in both directions, i.e.
    /// from Node1 to Node2 and from Node2 to Node1.
    ///
    std::unordered_set<ConnectionId> _connectivity;

    mutable std::unordered_map<NodeId, NodeGeometryData> _nodeGeometryData;

    ///
    /// @brief A convenience variable needed for generating unique node ids.
    ///
    NodeId _nextNodeId;
};

#endif  // SIMPLE_GRAPH_MODEL_HXX_7C4751E355BF
