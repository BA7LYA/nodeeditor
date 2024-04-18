///
/// @file DataFlowGraphModel.hpp
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-09
/// @copyright Copyright (c) 2024
///

#ifndef DATA_FLOW_GRAPH_MODEL_HXX_F63AAC96A797
#define DATA_FLOW_GRAPH_MODEL_HXX_F63AAC96A797

#include <memory>
#include <QJsonObject>

#include "AbstractGraphModel.hxx"
#include "ConnectionIdUtils.hxx"
#include "Export.hxx"
#include "NodeDelegateModelRegistry.hxx"
#include "Serializable.hxx"
#include "StyleCollection.hxx"

namespace QtNodes {

class NODE_EDITOR_PUBLIC DataFlowGraphModel
    : public AbstractGraphModel
    , public Serializable
{
    Q_OBJECT

public:
    struct NodeGeometryData
    {
        QSize   size;
        QPointF pos;
    };

    DataFlowGraphModel(std::shared_ptr<NodeDelegateModelRegistry> registry);

    std::shared_ptr<NodeDelegateModelRegistry> dataModelRegistry()
    {
        return _registry;
    }

    std::unordered_set<NodeId> allNodeIds() const override;

    std::unordered_set<ConnectionId> allConnectionIds(const NodeId nodeId
    ) const override;

    std::unordered_set<ConnectionId> connections(
        NodeId    nodeId,
        PortType  portType,
        PortIndex portIndex
    ) const override;

    bool connectionExists(const ConnectionId connectionId) const override;

    NodeId addNode(const QString nodeType) override;

    bool connectionPossible(const ConnectionId connectionId) const override;

    void addConnection(const ConnectionId connectionId) override;

    bool nodeExists(const NodeId nodeId) const override;

    QVariant nodeData(NodeId nodeId, NodeRole role) const override;

    NodeFlags nodeFlags(NodeId nodeId) const override;

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
    QJsonObject save() const override;

    void loadNode(const QJsonObject& nodeJson) override;
    void load(const QJsonObject& json) override;

    ///
    /// @brief Fetches the NodeDelegateModel for the given `nodeId` and tries to
    /// cast the stored pointer to the given type.
    /// @tparam NodeDelegateModelType
    /// @param nodeId
    /// @return
    ///
    template<typename NodeDelegateModelType>
    NodeDelegateModelType* delegateModel(const NodeId nodeId)
    {
        auto it = _models.find(nodeId);
        if (it == _models.end())
        {
            return nullptr;
        }

        auto model = dynamic_cast<NodeDelegateModelType*>(it->second.get());

        return model;
    }

Q_SIGNALS:
    void inPortDataWasSet(const NodeId, const PortType, const PortIndex);

private:
    NodeId newNodeId() override
    {
        return _nextNodeId++;
    }

    void sendConnectionCreation(const ConnectionId connectionId);

    void sendConnectionDeletion(const ConnectionId connectionId);

private Q_SLOTS:
    ///
    /// Fuction is called in three cases:
    ///
    /// - By underlying NodeDelegateModel when a node has new data to propagate.
    ///   @see DataFlowGraphModel::addNode
    /// - When a new connection is created.
    ///   @see DataFlowGraphModel::addConnection
    /// - When a node restored from JSON an needs to send data downstream.
    ///   @see DataFlowGraphModel::loadNode
    ///
    void onOutPortDataUpdated(const NodeId nodeId, const PortIndex portIndex);

    ///
    /// @brief Function is called after detaching a connection.
    ///
    /// @param nodeId
    /// @param portIndex
    ///
    void propagateEmptyDataTo(const NodeId nodeId, const PortIndex portIndex);

private:
    std::shared_ptr<NodeDelegateModelRegistry> _registry;

    NodeId _nextNodeId;

    std::unordered_map<NodeId, std::unique_ptr<NodeDelegateModel>> _models;

    std::unordered_set<ConnectionId> _connectivity;

    mutable std::unordered_map<NodeId, NodeGeometryData> _nodeGeometryData;
};

}  // namespace QtNodes

#endif  // DATA_FLOW_GRAPH_MODEL_HXX_F63AAC96A797
