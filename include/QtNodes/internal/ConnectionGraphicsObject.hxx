///
/// @file ConnectionGraphicsObject.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef CONNECTION_GRAPHICS_OBJECT_HXX_19BB13F7D071
#define CONNECTION_GRAPHICS_OBJECT_HXX_19BB13F7D071

#include <QtCore/QUuid>
#include <QtWidgets/QGraphicsObject>
#include <utility>

#include "ConnectionState.hxx"
#include "Definitions.hxx"

class QGraphicsSceneMouseEvent;

namespace QtNodes {

class AbstractGraphModel;
class BasicGraphicsScene;

///
/// @brief Graphic Object for connection. Adds itself to scene.
///
class ConnectionGraphicsObject : public QGraphicsObject
{
    Q_OBJECT

public:
    ///
    /// @brief Needed for qgraphicsitem_cast
    ///
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

    ///
    /// @brief Updates the position of both ends.
    ///
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

#endif  // CONNECTION_GRAPHICS_OBJECT_HXX_19BB13F7D071
