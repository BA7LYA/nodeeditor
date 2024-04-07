#pragma once

#include <QtCore/QUuid>
#include <QtWidgets/QGraphicsObject>
#include <utility>

#include "QtNodes/ConnectionState.hxx"
#include "QtNodes/Definitions.hxx"

class QGraphicsSceneMouseEvent;

namespace QtNodes {

class AbstractGraphModel;
class BasicGraphicsScene;

/// Graphic Object for connection. Adds itself to scene
class ConnectionGraphicsObject : public QGraphicsObject
{
    Q_OBJECT

public:
    // Needed for qgraphicsitem_cast
    enum
    {
        Type = UserType + 2
    };

    int type() const override
    {
        return Type;
    }

public:
    ConnectionGraphicsObject(
        BasicGraphicsScene& scene,
        const ConnectionId  connectionId
    );

    ~ConnectionGraphicsObject() = default;

public:
    AbstractGraphModel& graphModel() const;

    BasicGraphicsScene* nodeScene() const;

    const ConnectionId& connectionId() const;

    QRectF boundingRect() const override;

    QPainterPath shape() const override;

    const QPointF& endPoint(PortType portType) const;

    QPointF out() const
    {
        return _out;
    }

    QPointF in() const
    {
        return _in;
    }

    std::pair<QPointF, QPointF> pointsC1C2() const;

    void setEndPoint(PortType portType, const QPointF& point);

    /// Updates the position of both ends
    void move();

    const ConnectionState& connectionState() const;

    ConnectionState& connectionState();

protected:
    void paint(
        QPainter*                       painter,
        const QStyleOptionGraphicsItem* option,
        QWidget*                        widget = 0
    ) override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

private:
    void initializePosition();

    void addGraphicsEffect();

    std::pair<QPointF, QPointF> pointsC1C2Horizontal() const;

    std::pair<QPointF, QPointF> pointsC1C2Vertical() const;

private:
    ConnectionId _connectionId;

    AbstractGraphModel& _graphModel;

    ConnectionState _connectionState;

    mutable QPointF _out;
    mutable QPointF _in;
};

}  // namespace QtNodes
