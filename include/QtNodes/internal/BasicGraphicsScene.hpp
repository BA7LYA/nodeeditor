#pragma once

#include <functional>
#include <memory>
#include <QtCore/QUuid>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QMenu>
#include <tuple>
#include <unordered_map>

#include "AbstractGraphModel.hpp"
#include "AbstractNodeGeometry.hpp"
#include "ConnectionIdHash.hpp"
#include "Definitions.hpp"
#include "Export.hpp"
#include "QUuidStdHash.hpp"

class QUndoStack;

namespace QtNodes {

class AbstractGraphModel;
class AbstractNodePainter;
class ConnectionGraphicsObject;
class NodeGraphicsObject;
class NodeStyle;

/// An instance of QGraphicsScene, holds connections and nodes.
class NODE_EDITOR_PUBLIC BasicGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    BasicGraphicsScene(
        AbstractGraphModel& graphModel,
        QObject*            parent = nullptr
    );

    // Scenes without models are not supported
    BasicGraphicsScene() = delete;

    ~BasicGraphicsScene();

public:
    /// @returns associated AbstractGraphModel.
    const AbstractGraphModel& graphModel() const;

    AbstractGraphModel& graphModel();

    AbstractNodeGeometry& nodeGeometry();

    AbstractNodePainter& nodePainter();

    void setNodePainter(std::unique_ptr<AbstractNodePainter> newPainter);

    QUndoStack& undoStack();

public:
    /// Creates a "draft" instance of ConnectionGraphicsObject.
    /**
     * The scene caches a "draft" connection which has one loose end.
     * After attachment the "draft" instance is deleted and instead a
     * normal "full" connection is created.
     * Function @returns the "draft" instance for further geometry
     * manipulations.
     */
    const std::unique_ptr<ConnectionGraphicsObject>& makeDraftConnection(
        const ConnectionId newConnectionId
    );

    /// Deletes "draft" connection.
    /**
     * The function is called when user releases the mouse button during
     * the construction of the new connection without attaching it to any
     * node.
     */
    void resetDraftConnection();

    /// Deletes all the nodes. Connections are removed automatically.
    void clearScene();

public:
    /// @returns NodeGraphicsObject associated with the given nodeId.
    /**
     * @returns nullptr when the object is not found.
     */
    NodeGraphicsObject* nodeGraphicsObject(NodeId nodeId);

    /// @returns ConnectionGraphicsObject corresponding to `connectionId`.
    /**
     * @returns `nullptr` when the object is not found.
     */
    ConnectionGraphicsObject* connectionGraphicsObject(ConnectionId connectionId
    );

    Qt::Orientation orientation() const
    {
        return _orientation;
    }

    void setOrientation(const Qt::Orientation orientation);

public:
    /// Can @return an instance of the scene context menu in subclass.
    /**
     * Default implementation returns `nullptr`.
     */
    virtual QMenu* createSceneMenu(const QPointF scenePos);

Q_SIGNALS:
    void modified(BasicGraphicsScene*);

    void nodeMoved(const NodeId nodeId, const QPointF& newLocation);

    void nodeClicked(const NodeId nodeId);

    void nodeSelected(const NodeId nodeId);

    void nodeDoubleClicked(const NodeId nodeId);

    void nodeHovered(const NodeId nodeId, const QPoint screenPos);

    void nodeHoverLeft(const NodeId nodeId);

    void connectionHovered(
        const ConnectionId connectionId,
        const QPoint       screenPos
    );

    void connectionHoverLeft(const ConnectionId connectionId);

    /// Signal allows showing custom context menu upon clicking a node.
    void nodeContextMenu(const NodeId nodeId, const QPointF pos);

private:
    /// @brief Creates Node and Connection graphics objects.
    /**
     * Function is used to populate an empty scene in the constructor. We
     * perform depth-first AbstractGraphModel traversal. The connections are
     * created by checking non-empty node `Out` ports.
     */
    void traverseGraphAndPopulateGraphicsObjects();

    /// Redraws adjacent nodes for given `connectionId`
    void updateAttachedNodes(
        const ConnectionId connectionId,
        const PortType     portType
    );

public Q_SLOTS:
    /// Slot called when the `connectionId` is erased form the
    /// AbstractGraphModel.
    void onConnectionDeleted(const ConnectionId connectionId);

    /// Slot called when the `connectionId` is created in the
    /// AbstractGraphModel.
    void onConnectionCreated(const ConnectionId connectionId);

    void onNodeDeleted(const NodeId nodeId);

    void onNodeCreated(const NodeId nodeId);

    void onNodePositionUpdated(const NodeId nodeId);

    void onNodeUpdated(const NodeId nodeId);

    void onNodeClicked(const NodeId nodeId);

    void onModelReset();

private:
    AbstractGraphModel& _graphModel;

    using UniqueNodeGraphicsObject = std::unique_ptr<NodeGraphicsObject>;

    using UniqueConnectionGraphicsObject
        = std::unique_ptr<ConnectionGraphicsObject>;

    std::unordered_map<NodeId, UniqueNodeGraphicsObject> _nodeGraphicsObjects;

    std::unordered_map<ConnectionId, UniqueConnectionGraphicsObject>
        _connectionGraphicsObjects;

    std::unique_ptr<ConnectionGraphicsObject> _draftConnection;

    std::unique_ptr<AbstractNodeGeometry> _nodeGeometry;

    std::unique_ptr<AbstractNodePainter> _nodePainter;

    bool _nodeDrag;

    QUndoStack* _undoStack;

    Qt::Orientation _orientation;
};

}  // namespace QtNodes
