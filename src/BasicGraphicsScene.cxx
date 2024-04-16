///
/// @file BasicGraphicsScene.cxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#include "BasicGraphicsScene.hxx"

#include <iostream>
#include <QtCore/QBuffer>
#include <QtCore/QByteArray>
#include <QtCore/QDataStream>
#include <QtCore/QFile>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QtGlobal>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QGraphicsSceneMoveEvent>
#include <queue>
#include <QUndoStack>
#include <stdexcept>
#include <unordered_set>
#include <utility>

#include "AbstractNodeGeometry.hxx"
#include "ConnectionGraphicsObject.hxx"
#include "ConnectionIdUtils.hxx"
#include "DefaultHorizontalNodeGeometry.hxx"
#include "DefaultNodePainter.hxx"
#include "DefaultVerticalNodeGeometry.hxx"
#include "GraphicsView.hxx"
#include "NodeGraphicsObject.hxx"

namespace QtNodes {

BasicGraphicsScene::BasicGraphicsScene(
    AbstractGraphModel& graphModel,
    QObject*            parent
)
    : QGraphicsScene(parent)
    , _graphModel(graphModel)
    , _nodeGeometry(std::make_unique<DefaultHorizontalNodeGeometry>(_graphModel)
      )
    , _nodePainter(std::make_unique<DefaultNodePainter>())
    , _nodeDrag(false)
    , _undoStack(new QUndoStack(this))
    , _orientation(Qt::Horizontal)
{
    setItemIndexMethod(QGraphicsScene::NoIndex);

    connect(
        &_graphModel,
        &AbstractGraphModel::connectionCreated,
        this,
        &BasicGraphicsScene::onConnectionCreated
    );

    connect(
        &_graphModel,
        &AbstractGraphModel::connectionDeleted,
        this,
        &BasicGraphicsScene::onConnectionDeleted
    );

    connect(
        &_graphModel,
        &AbstractGraphModel::nodeCreated,
        this,
        &BasicGraphicsScene::onNodeCreated
    );

    connect(
        &_graphModel,
        &AbstractGraphModel::nodeDeleted,
        this,
        &BasicGraphicsScene::onNodeDeleted
    );

    connect(
        &_graphModel,
        &AbstractGraphModel::nodePositionUpdated,
        this,
        &BasicGraphicsScene::onNodePositionUpdated
    );

    connect(
        &_graphModel,
        &AbstractGraphModel::nodeUpdated,
        this,
        &BasicGraphicsScene::onNodeUpdated
    );

    connect(
        this,
        &BasicGraphicsScene::nodeClicked,
        this,
        &BasicGraphicsScene::onNodeClicked
    );

    connect(
        &_graphModel,
        &AbstractGraphModel::modelReset,
        this,
        &BasicGraphicsScene::onModelReset
    );

    traverseGraphAndPopulateGraphicsObjects();
}

BasicGraphicsScene::~BasicGraphicsScene() = default;

const AbstractGraphModel& BasicGraphicsScene::graphModel() const
{
    return _graphModel;
}

AbstractGraphModel& BasicGraphicsScene::graphModel()
{
    return _graphModel;
}

AbstractNodeGeometry& BasicGraphicsScene::nodeGeometry()
{
    return *_nodeGeometry;
}

AbstractNodePainter& BasicGraphicsScene::nodePainter()
{
    return *_nodePainter;
}

void BasicGraphicsScene::setNodePainter(
    std::unique_ptr<AbstractNodePainter> newPainter
)
{
    _nodePainter = std::move(newPainter);
}

QUndoStack& BasicGraphicsScene::undoStack()
{
    return *_undoStack;
}

const std::unique_ptr<ConnectionGraphicsObject>& BasicGraphicsScene::
    makeDraftConnection(const ConnectionId incompleteConnectionId)
{
    _draftConnection = std::make_unique<ConnectionGraphicsObject>(
        *this,
        incompleteConnectionId
    );

    _draftConnection->grabMouse();

    return _draftConnection;
}

void BasicGraphicsScene::resetDraftConnection()
{
    _draftConnection.reset();
}

void BasicGraphicsScene::clearScene()
{
    const auto& allNodeIds = graphModel().allNodeIds();

    for (auto nodeId : allNodeIds)
    {
        graphModel().deleteNode(nodeId);
    }
}

NodeGraphicsObject* BasicGraphicsScene::nodeGraphicsObject(NodeId nodeId)
{
    NodeGraphicsObject* ngo = nullptr;

    auto it = _nodeGraphicsObjects.find(nodeId);
    if (it != _nodeGraphicsObjects.end())
    {
        ngo = it->second.get();
    }

    return ngo;
}

ConnectionGraphicsObject* BasicGraphicsScene::connectionGraphicsObject(
    ConnectionId connectionId
)
{
    ConnectionGraphicsObject* cgo = nullptr;

    auto it = _connectionGraphicsObjects.find(connectionId);
    if (it != _connectionGraphicsObjects.end())
    {
        cgo = it->second.get();
    }

    return cgo;
}

void BasicGraphicsScene::setOrientation(const Qt::Orientation orientation)
{
    if (_orientation != orientation)
    {
        _orientation = orientation;

        switch (_orientation)
        {
        case Qt::Horizontal:
            _nodeGeometry
                = std::make_unique<DefaultHorizontalNodeGeometry>(_graphModel);
            break;
        case Qt::Vertical:
            _nodeGeometry
                = std::make_unique<DefaultVerticalNodeGeometry>(_graphModel);
            break;
        }

        onModelReset();
    }
}

QMenu* BasicGraphicsScene::createSceneMenu(const QPointF scenePos)
{
    Q_UNUSED(scenePos);
    return nullptr;
}

void BasicGraphicsScene::traverseGraphAndPopulateGraphicsObjects()
{
    auto allNodeIds = _graphModel.allNodeIds();

    // First create all the nodes.
    for (const NodeId nodeId : allNodeIds)
    {
        _nodeGraphicsObjects[nodeId]
            = std::make_unique<NodeGraphicsObject>(*this, nodeId);
    }

    // Then for each node check output connections and insert them.
    for (const NodeId nodeId : allNodeIds)
    {
        auto nOutPorts
            = _graphModel.nodeData<PortCount>(nodeId, NodeRole::OutPortCount);

        for (PortIndex index = 0; index < nOutPorts; ++index)
        {
            const auto& outConnectionIds
                = _graphModel.connections(nodeId, PortType::Out, index);

            for (auto cid : outConnectionIds)
            {
                _connectionGraphicsObjects[cid]
                    = std::make_unique<ConnectionGraphicsObject>(*this, cid);
            }
        }
    }
}

void BasicGraphicsScene::updateAttachedNodes(
    const ConnectionId connectionId,
    const PortType     portType
)
{
    auto node = nodeGraphicsObject(getNodeId(portType, connectionId));

    if (node)
    {
        node->update();
    }
}

void BasicGraphicsScene::onConnectionDeleted(const ConnectionId connectionId)
{
    auto it = _connectionGraphicsObjects.find(connectionId);
    if (it != _connectionGraphicsObjects.end())
    {
        _connectionGraphicsObjects.erase(it);
    }

    // TODO: do we need it?
    if (_draftConnection && _draftConnection->connectionId() == connectionId)
    {
        _draftConnection.reset();
    }

    updateAttachedNodes(connectionId, PortType::Out);
    updateAttachedNodes(connectionId, PortType::In);

    Q_EMIT modified(this);
}

void BasicGraphicsScene::onConnectionCreated(const ConnectionId connectionId)
{
    _connectionGraphicsObjects[connectionId]
        = std::make_unique<ConnectionGraphicsObject>(*this, connectionId);

    updateAttachedNodes(connectionId, PortType::Out);
    updateAttachedNodes(connectionId, PortType::In);

    Q_EMIT modified(this);
}

void BasicGraphicsScene::onNodeDeleted(const NodeId nodeId)
{
    auto it = _nodeGraphicsObjects.find(nodeId);
    if (it != _nodeGraphicsObjects.end())
    {
        _nodeGraphicsObjects.erase(it);

        Q_EMIT modified(this);
    }
}

void BasicGraphicsScene::onNodeCreated(const NodeId nodeId)
{
    _nodeGraphicsObjects[nodeId]
        = std::make_unique<NodeGraphicsObject>(*this, nodeId);

    Q_EMIT modified(this);
}

void BasicGraphicsScene::onNodePositionUpdated(const NodeId nodeId)
{
    auto node = nodeGraphicsObject(nodeId);
    if (node)
    {
        node->setPos(
            _graphModel.nodeData(nodeId, NodeRole::Position).value<QPointF>()
        );
        node->update();
        _nodeDrag = true;
    }
}

void BasicGraphicsScene::onNodeUpdated(const NodeId nodeId)
{
    auto node = nodeGraphicsObject(nodeId);

    if (node)
    {
        node->setGeometryChanged();

        _nodeGeometry->recomputeSize(nodeId);

        node->update();
        node->moveConnections();
    }
}

void BasicGraphicsScene::onNodeClicked(const NodeId nodeId)
{
    if (_nodeDrag)
    {
        Q_EMIT nodeMoved(
            nodeId,
            _graphModel.nodeData(nodeId, NodeRole::Position).value<QPointF>()
        );
        Q_EMIT modified(this);
    }
    _nodeDrag = false;
}

void BasicGraphicsScene::onModelReset()
{
    _connectionGraphicsObjects.clear();
    _nodeGraphicsObjects.clear();

    clear();

    traverseGraphAndPopulateGraphicsObjects();
}

}  // namespace QtNodes
