///
/// @file NodeGraphicsObject.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef NODE_GRAPHICS_OBJECT_HXX_F5FB4FEE6403
#define NODE_GRAPHICS_OBJECT_HXX_F5FB4FEE6403

#include <QtCore/QUuid>
#include <QtWidgets/QGraphicsObject>

#include "NodeState.hxx"

class QGraphicsProxyWidget;

namespace QtNodes {

class BasicGraphicsScene;
class AbstractGraphModel;

class NodeGraphicsObject : public QGraphicsObject
{
    Q_OBJECT

public:
    ///
    /// @brief Needed for `qgraphicsitem_cast`.
    ///
    enum
    {
        Type = UserType + 1
    };

    int type() const override
    {
        return Type;
    }

public:
    NodeGraphicsObject(BasicGraphicsScene& scene, NodeId node);

    ~NodeGraphicsObject() override = default;

public:
    AbstractGraphModel& graphModel() const;

    BasicGraphicsScene* nodeScene() const;

    NodeId nodeId()
    {
        return _nodeId;
    }

    NodeId nodeId() const
    {
        return _nodeId;
    }

    NodeState& nodeState()
    {
        return _nodeState;
    }

    const NodeState& nodeState() const
    {
        return _nodeState;
    }

    QRectF boundingRect() const override;

    void setGeometryChanged();

    ///
    /// @brief Visits all attached connections and corrects their corresponding
    /// end points.
    ///
    void moveConnections() const;

    ///
    /// @brief Repaints the node once with reacting ports.
    ///
    /// @param cgo
    ///
    void reactToConnection(const ConnectionGraphicsObject* cgo);

protected:
    void paint(
        QPainter*                       painter,
        const QStyleOptionGraphicsItem* option,
        QWidget*                        widget = 0
    ) override;

    QVariant itemChange(GraphicsItemChange change, const QVariant& value)
        override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

    void hoverMoveEvent(QGraphicsSceneHoverEvent*) override;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

private:
    void embedQWidget();

    void setLockedState();

private:
    NodeId _nodeId;

    AbstractGraphModel& _graphModel;

    NodeState _nodeState;

    ///
    /// @brief either nullptr or owned by parent QGraphicsItem.
    ///
    QGraphicsProxyWidget* _proxyWidget;
};

}  // namespace QtNodes

#endif  // NODE_GRAPHICS_OBJECT_HXX_F5FB4FEE6403
