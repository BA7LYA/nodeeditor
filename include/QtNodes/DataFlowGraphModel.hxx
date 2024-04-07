#pragma once

#include <memory>
#include <QJsonObject>

#include "QtNodes/AbstractGraphModel.hxx"
#include "QtNodes/ConnectionIdUtils.hxx"
#include "QtNodes/Export.hxx"
#include "QtNodes/NodeDelegateModelRegistry.hxx"
#include "QtNodes/Serializable.hxx"
#include "QtNodes/StyleCollection.hxx"

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

public:
    DataFlowGraphModel(std::shared_ptr<NodeDelegateModelRegistry> registry);

    std::shared_ptr<NodeDelegateModelRegistry> dataModelRegistry()
    {
        return _registry;
    }

public:
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

    /**
     * Fetches the NodeDelegateModel for the given `nodeId` and tries to cast
     * the stored pointer to the given type
     */
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
    /**
     * Fuction is called in three cases:
     *
     * - By underlying NodeDelegateModel when a node has new data to propagate.
     *   @see DataFlowGraphModel::addNode
     * - When a new connection is created.
     *   @see DataFlowGraphModel::addConnection
     * - When a node restored from JSON an needs to send data downstream.
     *   @see DataFlowGraphModel::loadNode
     */
    void onOutPortDataUpdated(const NodeId nodeId, const PortIndex portIndex);

    /// Function is called after detaching a connection.
    void propagateEmptyDataTo(const NodeId nodeId, const PortIndex portIndex);

private:
    std::shared_ptr<NodeDelegateModelRegistry> _registry;

    NodeId _nextNodeId;

    std::unordered_map<NodeId, std::unique_ptr<NodeDelegateModel>> _models;

    std::unordered_set<ConnectionId> _connectivity;

    mutable std::unordered_map<NodeId, NodeGeometryData> _nodeGeometryData;
};

}  // namespace QtNodes
