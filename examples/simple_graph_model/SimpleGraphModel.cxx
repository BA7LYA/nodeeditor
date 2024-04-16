///
/// @file SimpleGraphModel.cxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#include "SimpleGraphModel.hxx"

SimpleGraphModel::SimpleGraphModel()
    : _nextNodeId { 0 }
{
}

SimpleGraphModel::~SimpleGraphModel()
{
    //
}

std::unordered_set<NodeId> SimpleGraphModel::allNodeIds() const
{
    return _nodeIds;
}

std::unordered_set<ConnectionId> SimpleGraphModel::allConnectionIds(
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

std::unordered_set<ConnectionId> SimpleGraphModel::connections(
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

bool SimpleGraphModel::connectionExists(const ConnectionId connectionId) const
{
    return (_connectivity.find(connectionId) != _connectivity.end());
}

NodeId SimpleGraphModel::addNode(const QString nodeType)
{
    NodeId newId = newNodeId();
    // Create new node.
    _nodeIds.insert(newId);

    Q_EMIT nodeCreated(newId);

    return newId;
}

bool SimpleGraphModel::connectionPossible(const ConnectionId connectionId) const
{
    return _connectivity.find(connectionId) == _connectivity.end();
}

void SimpleGraphModel::addConnection(const ConnectionId connectionId)
{
    _connectivity.insert(connectionId);

    Q_EMIT connectionCreated(connectionId);
}

bool SimpleGraphModel::nodeExists(const NodeId nodeId) const
{
    return (_nodeIds.find(nodeId) != _nodeIds.end());
}

QVariant SimpleGraphModel::nodeData(NodeId nodeId, NodeRole role) const
{
    Q_UNUSED(nodeId);

    QVariant result;

    switch (role)
    {
    case NodeRole::Type: result = QString("Default Node Type"); break;
    case NodeRole::Position: result = _nodeGeometryData[nodeId].pos; break;
    case NodeRole::Size: result = _nodeGeometryData[nodeId].size; break;
    case NodeRole::CaptionVisible: result = true; break;
    case NodeRole::Caption: result = QString("Node"); break;
    case NodeRole::Style:
    {
        auto style = StyleCollection::nodeStyle();
        result     = style.toJson().toVariantMap();
    }
    break;
    case NodeRole::InternalData: break;
    case NodeRole::InPortCount: result = 1u; break;
    case NodeRole::OutPortCount: result = 1u; break;
    case NodeRole::Widget: result = QVariant(); break;
    }

    return result;
}

bool SimpleGraphModel::setNodeData(NodeId nodeId, NodeRole role, QVariant value)
{
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

QVariant SimpleGraphModel::portData(
    NodeId    nodeId,
    PortType  portType,
    PortIndex portIndex,
    PortRole  role
) const
{
    switch (role)
    {
    case PortRole::Data: return QVariant(); break;
    case PortRole::DataType: return QVariant(); break;
    case PortRole::ConnectionPolicyRole:
        return QVariant::fromValue(ConnectionPolicy::One);
        break;
    case PortRole::CaptionVisible: return true; break;
    case PortRole::Caption:
        if (portType == PortType::In)
        {
            return QString::fromUtf8("Port In");
        }
        else
        {
            return QString::fromUtf8("Port Out");
        }

        break;
    }

    return QVariant();
}

bool SimpleGraphModel::setPortData(
    NodeId          nodeId,
    PortType        portType,
    PortIndex       portIndex,
    const QVariant& value,
    PortRole        role
)
{
    Q_UNUSED(nodeId);
    Q_UNUSED(portType);
    Q_UNUSED(portIndex);
    Q_UNUSED(value);
    Q_UNUSED(role);

    return false;
}

bool SimpleGraphModel::deleteConnection(const ConnectionId connectionId)
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
        Q_EMIT connectionDeleted(connectionId);
    }

    return disconnected;
}

bool SimpleGraphModel::deleteNode(const NodeId nodeId)
{
    // Delete connections to this node first.
    auto connectionIds = allConnectionIds(nodeId);

    for (auto& cId : connectionIds)
    {
        deleteConnection(cId);
    }

    _nodeIds.erase(nodeId);
    _nodeGeometryData.erase(nodeId);

    Q_EMIT nodeDeleted(nodeId);

    return true;
}

QJsonObject SimpleGraphModel::saveNode(const NodeId nodeId) const
{
    QJsonObject nodeJson;

    nodeJson["id"] = static_cast<qint64>(nodeId);

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

void SimpleGraphModel::loadNode(const QJsonObject& nodeJson)
{
    NodeId restoredNodeId = static_cast<NodeId>(nodeJson["id"].toInt());

    // Next NodeId must be larger that any id existing in the graph
    _nextNodeId = std::max(_nextNodeId, restoredNodeId + 1);

    // Create new node.
    _nodeIds.insert(restoredNodeId);

    Q_EMIT nodeCreated(restoredNodeId);

    {
        QJsonObject   posJson = nodeJson["position"].toObject();
        const QPointF pos(posJson["x"].toDouble(), posJson["y"].toDouble());

        setNodeData(restoredNodeId, NodeRole::Position, pos);
    }
}
