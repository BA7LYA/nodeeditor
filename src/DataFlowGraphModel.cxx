#include "QtNodes/DataFlowGraphModel.hxx"

#include <QJsonArray>
#include <stdexcept>

#include "QtNodes/ConnectionIdHash.hxx"

namespace QtNodes {

DataFlowGraphModel::DataFlowGraphModel(
    std::shared_ptr<NodeDelegateModelRegistry> registry
)
    : _registry(std::move(registry))
    , _nextNodeId { 0 }
{
}

std::unordered_set<NodeId> DataFlowGraphModel::allNodeIds() const
{
    std::unordered_set<NodeId> nodeIds;
    for_each(
        _models.begin(),
        _models.end(),
        [&nodeIds](const auto& p) { nodeIds.insert(p.first); }
    );

    return nodeIds;
}

std::unordered_set<ConnectionId> DataFlowGraphModel::allConnectionIds(
    const NodeId nodeId
) const
{
    std::unordered_set<ConnectionId> result;

    std::copy_if(
        _connectivity.begin(),
        _connectivity.end(),
        std::inserter(result, std::end(result)),
        [&nodeId](const ConnectionId& cid)
        { return cid.inNodeId == nodeId || cid.outNodeId == nodeId; }
    );

    return result;
}

std::unordered_set<ConnectionId> DataFlowGraphModel::connections(
    NodeId    nodeId,
    PortType  portType,
    PortIndex portIndex
) const
{
    std::unordered_set<ConnectionId> result;

    std::copy_if(
        _connectivity.begin(),
        _connectivity.end(),
        std::inserter(result, std::end(result)),
        [&portType, &portIndex, &nodeId](const ConnectionId& cid)
        {
            return (
                getNodeId(portType, cid) == nodeId
                && getPortIndex(portType, cid) == portIndex
            );
        }
    );

    return result;
}

bool DataFlowGraphModel::connectionExists(const ConnectionId connectionId) const
{
    return (_connectivity.find(connectionId) != _connectivity.end());
}

NodeId DataFlowGraphModel::addNode(const QString nodeType)
{
    std::unique_ptr<NodeDelegateModel> model = _registry->create(nodeType);

    if (model)
    {
        NodeId newId = newNodeId();

        connect(
            model.get(),
            &NodeDelegateModel::dataUpdated,
            [newId, this](const PortIndex portIndex)
            { onOutPortDataUpdated(newId, portIndex); }
        );

        connect(
            model.get(),
            &NodeDelegateModel::portsAboutToBeDeleted,
            this,
            [newId, this](
                const PortType  portType,
                const PortIndex first,
                const PortIndex last
            ) { portsAboutToBeDeleted(newId, portType, first, last); }
        );

        connect(
            model.get(),
            &NodeDelegateModel::portsDeleted,
            this,
            &DataFlowGraphModel::portsDeleted
        );

        connect(
            model.get(),
            &NodeDelegateModel::portsAboutToBeInserted,
            this,
            [newId, this](
                const PortType  portType,
                const PortIndex first,
                const PortIndex last
            ) { portsAboutToBeInserted(newId, portType, first, last); }
        );

        connect(
            model.get(),
            &NodeDelegateModel::portsInserted,
            this,
            &DataFlowGraphModel::portsInserted
        );

        _models[newId] = std::move(model);

        Q_EMIT nodeCreated(newId);

        return newId;
    }

    return InvalidNodeId;
}

bool DataFlowGraphModel::connectionPossible(const ConnectionId connectionId
) const
{
    auto getDataType = [&](const PortType portType)
    {
        return portData(
                   getNodeId(portType, connectionId),
                   portType,
                   getPortIndex(portType, connectionId),
                   PortRole::DataType
        )
            .value<NodeDataType>();
    };

    auto portVacant = [&](const PortType portType)
    {
        NodeId const    nodeId    = getNodeId(portType, connectionId);
        PortIndex const portIndex = getPortIndex(portType, connectionId);
        auto const      connected = connections(nodeId, portType, portIndex);

        auto policy = portData(
                          nodeId,
                          portType,
                          portIndex,
                          PortRole::ConnectionPolicyRole
        )
                          .value<ConnectionPolicy>();

        return connected.empty() || (policy == ConnectionPolicy::Many);
    };

    return getDataType(PortType::Out).id == getDataType(PortType::In).id
        && portVacant(PortType::Out) && portVacant(PortType::In);
}

void DataFlowGraphModel::addConnection(const ConnectionId connectionId)
{
    _connectivity.insert(connectionId);

    sendConnectionCreation(connectionId);

    const QVariant portDataToPropagate = portData(
        connectionId.outNodeId,
        PortType::Out,
        connectionId.outPortIndex,
        PortRole::Data
    );

    setPortData(
        connectionId.inNodeId,
        PortType::In,
        connectionId.inPortIndex,
        portDataToPropagate,
        PortRole::Data
    );
}

void DataFlowGraphModel::sendConnectionCreation(const ConnectionId connectionId)
{
    Q_EMIT connectionCreated(connectionId);

    auto iti = _models.find(connectionId.inNodeId);
    auto ito = _models.find(connectionId.outNodeId);
    if (iti != _models.end() && ito != _models.end())
    {
        auto& modeli = iti->second;
        auto& modelo = ito->second;
        modeli->inputConnectionCreated(connectionId);
        modelo->outputConnectionCreated(connectionId);
    }
}

void DataFlowGraphModel::sendConnectionDeletion(const ConnectionId connectionId)
{
    Q_EMIT connectionDeleted(connectionId);

    auto iti = _models.find(connectionId.inNodeId);
    auto ito = _models.find(connectionId.outNodeId);
    if (iti != _models.end() && ito != _models.end())
    {
        auto& modeli = iti->second;
        auto& modelo = ito->second;
        modeli->inputConnectionDeleted(connectionId);
        modelo->outputConnectionDeleted(connectionId);
    }
}

bool DataFlowGraphModel::nodeExists(const NodeId nodeId) const
{
    return (_models.find(nodeId) != _models.end());
}

QVariant DataFlowGraphModel::nodeData(NodeId nodeId, NodeRole role) const
{
    QVariant result;

    auto it = _models.find(nodeId);
    if (it == _models.end())
    {
        return result;
    }

    auto& model = it->second;

    switch (role)
    {
    case NodeRole::Type: result = model->name(); break;

    case NodeRole::Position: result = _nodeGeometryData[nodeId].pos; break;

    case NodeRole::Size: result = _nodeGeometryData[nodeId].size; break;

    case NodeRole::CaptionVisible: result = model->captionVisible(); break;

    case NodeRole::Caption: result = model->caption(); break;

    case NodeRole::Style:
    {
        auto style = StyleCollection::nodeStyle();
        result     = style.toJson().toVariantMap();
    }
    break;

    case NodeRole::InternalData:
    {
        QJsonObject nodeJson;

        nodeJson["internal-data"] = _models.at(nodeId)->save();

        result = nodeJson.toVariantMap();
        break;
    }

    case NodeRole::InPortCount: result = model->nPorts(PortType::In); break;

    case NodeRole::OutPortCount: result = model->nPorts(PortType::Out); break;

    case NodeRole::Widget:
    {
        auto w = model->embeddedWidget();
        result = QVariant::fromValue(w);
    }
    break;
    }

    return result;
}

NodeFlags DataFlowGraphModel::nodeFlags(NodeId nodeId) const
{
    auto it = _models.find(nodeId);

    if (it != _models.end() && it->second->resizable())
    {
        return NodeFlag::Resizable;
    }

    return NodeFlag::NoFlags;
}

bool DataFlowGraphModel::setNodeData(
    NodeId   nodeId,
    NodeRole role,
    QVariant value
)
{
    Q_UNUSED(nodeId);
    Q_UNUSED(role);
    Q_UNUSED(value);

    bool result = false;

    switch (role)
    {
    case NodeRole::Type: break;
    case NodeRole::Position:
    {
        _nodeGeometryData[nodeId].pos = value.value<QPointF>();

        Q_EMIT nodePositionUpdated(nodeId);

        result = true;
    }
    break;

    case NodeRole::Size:
    {
        _nodeGeometryData[nodeId].size = value.value<QSize>();
        result                         = true;
    }
    break;

    case NodeRole::CaptionVisible: break;

    case NodeRole::Caption: break;

    case NodeRole::Style: break;

    case NodeRole::InternalData: break;

    case NodeRole::InPortCount: break;

    case NodeRole::OutPortCount: break;

    case NodeRole::Widget: break;
    }

    return result;
}

QVariant DataFlowGraphModel::portData(
    NodeId    nodeId,
    PortType  portType,
    PortIndex portIndex,
    PortRole  role
) const
{
    QVariant result;

    auto it = _models.find(nodeId);
    if (it == _models.end())
    {
        return result;
    }

    auto& model = it->second;

    switch (role)
    {
    case PortRole::Data:
        if (portType == PortType::Out)
        {
            result = QVariant::fromValue(model->outData(portIndex));
        }
        break;

    case PortRole::DataType:
        result = QVariant::fromValue(model->dataType(portType, portIndex));
        break;

    case PortRole::ConnectionPolicyRole:
        result = QVariant::fromValue(
            model->portConnectionPolicy(portType, portIndex)
        );
        break;

    case PortRole::CaptionVisible:
        result = model->portCaptionVisible(portType, portIndex);
        break;

    case PortRole::Caption:
        result = model->portCaption(portType, portIndex);

        break;
    }

    return result;
}

bool DataFlowGraphModel::setPortData(
    NodeId          nodeId,
    PortType        portType,
    PortIndex       portIndex,
    const QVariant& value,
    PortRole        role
)
{
    Q_UNUSED(nodeId);

    QVariant result;

    auto it = _models.find(nodeId);
    if (it == _models.end())
    {
        return false;
    }

    auto& model = it->second;

    switch (role)
    {
    case PortRole::Data:
        if (portType == PortType::In)
        {
            model->setInData(
                value.value<std::shared_ptr<NodeData>>(),
                portIndex
            );

            // Triggers repainting on the scene.
            Q_EMIT inPortDataWasSet(nodeId, portType, portIndex);
        }
        break;

    default: break;
    }

    return false;
}

bool DataFlowGraphModel::deleteConnection(const ConnectionId connectionId)
{
    bool disconnected = false;

    auto it = _connectivity.find(connectionId);

    if (it != _connectivity.end())
    {
        disconnected = true;

        _connectivity.erase(it);
    }

    if (disconnected)
    {
        sendConnectionDeletion(connectionId);

        propagateEmptyDataTo(
            getNodeId(PortType::In, connectionId),
            getPortIndex(PortType::In, connectionId)
        );
    }

    return disconnected;
}

bool DataFlowGraphModel::deleteNode(const NodeId nodeId)
{
    // Delete connections to this node first.
    auto connectionIds = allConnectionIds(nodeId);
    for (auto& cId : connectionIds)
    {
        deleteConnection(cId);
    }

    _nodeGeometryData.erase(nodeId);
    _models.erase(nodeId);

    Q_EMIT nodeDeleted(nodeId);

    return true;
}

QJsonObject DataFlowGraphModel::saveNode(const NodeId nodeId) const
{
    QJsonObject nodeJson;

    nodeJson["id"] = static_cast<qint64>(nodeId);

    nodeJson["internal-data"] = _models.at(nodeId)->save();

    {
        const QPointF pos
            = nodeData(nodeId, NodeRole::Position).value<QPointF>();

        QJsonObject posJson;
        posJson["x"]         = pos.x();
        posJson["y"]         = pos.y();
        nodeJson["position"] = posJson;
    }

    return nodeJson;
}

QJsonObject DataFlowGraphModel::save() const
{
    QJsonObject sceneJson;

    QJsonArray nodesJsonArray;
    for (const auto nodeId : allNodeIds())
    {
        nodesJsonArray.append(saveNode(nodeId));
    }
    sceneJson["nodes"] = nodesJsonArray;

    QJsonArray connJsonArray;
    for (const auto& cid : _connectivity)
    {
        connJsonArray.append(toJson(cid));
    }
    sceneJson["connections"] = connJsonArray;

    return sceneJson;
}

void DataFlowGraphModel::loadNode(const QJsonObject& nodeJson)
{
    // Possibility of the id clash when reading it from json and not generating
    // a new value.
    // 1. When restoring a scene from a file.
    // Conflict is not possible because the scene must be cleared by the time of
    // loading.
    // 2. When undoing the deletion command.  Conflict is not possible
    // because all the new ids were created past the removed nodes.
    NodeId restoredNodeId = nodeJson["id"].toInt();

    _nextNodeId = std::max(_nextNodeId, restoredNodeId + 1);

    const QJsonObject internalDataJson = nodeJson["internal-data"].toObject();

    QString delegateModelName = internalDataJson["model-name"].toString();

    std::unique_ptr<NodeDelegateModel> model
        = _registry->create(delegateModelName);

    if (model)
    {
        connect(
            model.get(),
            &NodeDelegateModel::dataUpdated,
            [restoredNodeId, this](const PortIndex portIndex)
            { onOutPortDataUpdated(restoredNodeId, portIndex); }
        );

        _models[restoredNodeId] = std::move(model);

        Q_EMIT nodeCreated(restoredNodeId);

        QJsonObject   posJson = nodeJson["position"].toObject();
        const QPointF pos(posJson["x"].toDouble(), posJson["y"].toDouble());

        setNodeData(restoredNodeId, NodeRole::Position, pos);

        _models[restoredNodeId]->load(internalDataJson);
    }
    else
    {
        throw std::logic_error(
            std::string("No registered model with name ")
            + delegateModelName.toLocal8Bit().data()
        );
    }
}

void DataFlowGraphModel::load(const QJsonObject& jsonDocument)
{
    QJsonArray nodesJsonArray = jsonDocument["nodes"].toArray();

    for (QJsonValueRef nodeJson : nodesJsonArray)
    {
        loadNode(nodeJson.toObject());
    }

    QJsonArray connectionJsonArray = jsonDocument["connections"].toArray();

    for (QJsonValueRef connection : connectionJsonArray)
    {
        QJsonObject connJson = connection.toObject();

        ConnectionId connId = fromJson(connJson);

        // Restore the connection
        addConnection(connId);
    }
}

void DataFlowGraphModel::onOutPortDataUpdated(
    const NodeId    nodeId,
    const PortIndex portIndex
)
{
    const std::unordered_set<ConnectionId>& connected
        = connections(nodeId, PortType::Out, portIndex);

    const QVariant portDataToPropagate
        = portData(nodeId, PortType::Out, portIndex, PortRole::Data);

    for (const auto& cn : connected)
    {
        setPortData(
            cn.inNodeId,
            PortType::In,
            cn.inPortIndex,
            portDataToPropagate,
            PortRole::Data
        );
    }
}

void DataFlowGraphModel::propagateEmptyDataTo(
    const NodeId    nodeId,
    const PortIndex portIndex
)
{
    QVariant emptyData {};

    setPortData(nodeId, PortType::In, portIndex, emptyData, PortRole::Data);
}

}  // namespace QtNodes
