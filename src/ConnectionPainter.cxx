#include "QtNodes/ConnectionPainter.hxx"

#include <QtGui/QIcon>

#include "QtNodes/AbstractGraphModel.hxx"
#include "QtNodes/ConnectionGraphicsObject.hxx"
#include "QtNodes/ConnectionState.hxx"
#include "QtNodes/Definitions.hxx"
#include "QtNodes/NodeData.hxx"
#include "QtNodes/StyleCollection.hxx"

namespace QtNodes {

static QPainterPath cubicPath(const ConnectionGraphicsObject& connection)
{
    const QPointF& in  = connection.endPoint(PortType::In);
    const QPointF& out = connection.endPoint(PortType::Out);

    const auto c1c2 = connection.pointsC1C2();

    // cubic spline
    QPainterPath cubic(out);

    cubic.cubicTo(c1c2.first, c1c2.second, in);

    return cubic;
}

QPainterPath ConnectionPainter::getPainterStroke(
    const ConnectionGraphicsObject& connection
)
{
    auto cubic = cubicPath(connection);

    const QPointF& out = connection.endPoint(PortType::Out);
    QPainterPath   result(out);

    unsigned segments = 20;

    for (auto i = 0ul; i < segments; ++i)
    {
        double ratio = double(i + 1) / segments;
        result.lineTo(cubic.pointAtPercent(ratio));
    }

    QPainterPathStroker stroker;
    stroker.setWidth(10.0);

    return stroker.createStroke(result);
}

#ifdef NODE_DEBUG_DRAWING
static void debugDrawing(QPainter* painter, const ConnectionGraphicsObject& cgo)
{
    Q_UNUSED(painter);

    {
        const QPointF& in  = cgo.endPoint(PortType::In);
        const QPointF& out = cgo.endPoint(PortType::Out);

        const auto points = cgo.pointsC1C2();

        painter->setPen(Qt::red);
        painter->setBrush(Qt::red);

        painter->drawLine(QLineF(out, points.first));
        painter->drawLine(QLineF(points.first, points.second));
        painter->drawLine(QLineF(points.second, in));
        painter->drawEllipse(points.first, 3, 3);
        painter->drawEllipse(points.second, 3, 3);

        painter->setBrush(Qt::NoBrush);
        painter->drawPath(cubicPath(cgo));
    }

    {
        painter->setPen(Qt::yellow);
        painter->drawRect(cgo.boundingRect());
    }
}

#endif

static void drawSketchLine(
    QPainter*                       painter,
    const ConnectionGraphicsObject& cgo
)
{
    const ConnectionState& state = cgo.connectionState();

    if (state.requiresPort())
    {
        const auto& connectionStyle
            = QtNodes::StyleCollection::connectionStyle();

        QPen pen;
        pen.setWidth(connectionStyle.constructionLineWidth());
        pen.setColor(connectionStyle.constructionColor());
        pen.setStyle(Qt::DashLine);

        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);

        auto cubic = cubicPath(cgo);

        // cubic spline
        painter->drawPath(cubic);
    }
}

static void drawHoveredOrSelected(
    QPainter*                       painter,
    const ConnectionGraphicsObject& cgo
)
{
    const bool hovered  = cgo.connectionState().hovered();
    const bool selected = cgo.isSelected();

    // drawn as a fat background
    if (hovered || selected)
    {
        const auto& connectionStyle
            = QtNodes::StyleCollection::connectionStyle();

        const double lineWidth = connectionStyle.lineWidth();

        QPen pen;
        pen.setWidth(2 * lineWidth);
        pen.setColor(
            selected ? connectionStyle.selectedHaloColor()
                     : connectionStyle.hoveredColor()
        );

        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);

        // cubic spline
        const auto cubic = cubicPath(cgo);
        painter->drawPath(cubic);
    }
}

static void drawNormalLine(
    QPainter*                       painter,
    const ConnectionGraphicsObject& cgo
)
{
    const ConnectionState& state = cgo.connectionState();

    if (state.requiresPort())
    {
        return;
    }

    // colors

    const auto& connectionStyle = QtNodes::StyleCollection::connectionStyle();

    QColor normalColorOut = connectionStyle.normalColor();
    QColor normalColorIn  = connectionStyle.normalColor();
    QColor selectedColor  = connectionStyle.selectedColor();

    bool useGradientColor = false;

    const AbstractGraphModel& graphModel = cgo.graphModel();

    if (connectionStyle.useDataDefinedColors())
    {
        using QtNodes::PortType;

        const auto cId = cgo.connectionId();

        auto dataTypeOut = graphModel
                               .portData(
                                   cId.outNodeId,
                                   PortType::Out,
                                   cId.outPortIndex,
                                   PortRole::DataType
                               )
                               .value<NodeDataType>();

        auto dataTypeIn = graphModel
                              .portData(
                                  cId.inNodeId,
                                  PortType::In,
                                  cId.inPortIndex,
                                  PortRole::DataType
                              )
                              .value<NodeDataType>();

        useGradientColor = (dataTypeOut.id != dataTypeIn.id);

        normalColorOut = connectionStyle.normalColor(dataTypeOut.id);
        normalColorIn  = connectionStyle.normalColor(dataTypeIn.id);
        selectedColor  = normalColorOut.darker(200);
    }

    // geometry

    const double lineWidth = connectionStyle.lineWidth();

    // draw normal line
    QPen p;

    p.setWidth(lineWidth);

    const bool selected = cgo.isSelected();

    auto cubic = cubicPath(cgo);
    if (useGradientColor)
    {
        painter->setBrush(Qt::NoBrush);

        QColor cOut = normalColorOut;
        if (selected)
        {
            cOut = cOut.darker(200);
        }
        p.setColor(cOut);
        painter->setPen(p);

        const unsigned int segments = 60;

        for (unsigned int i = 0ul; i < segments; ++i)
        {
            double ratioPrev = double(i) / segments;
            double ratio     = double(i + 1) / segments;

            if (i == segments / 2)
            {
                QColor cIn = normalColorIn;
                if (selected)
                {
                    cIn = cIn.darker(200);
                }

                p.setColor(cIn);
                painter->setPen(p);
            }
            painter->drawLine(
                cubic.pointAtPercent(ratioPrev),
                cubic.pointAtPercent(ratio)
            );
        }

        {
            QIcon icon(":convert.png");

            QPixmap pixmap = icon.pixmap(QSize(22, 22));
            painter->drawPixmap(
                cubic.pointAtPercent(0.50)
                    - QPoint(pixmap.width() / 2, pixmap.height() / 2),
                pixmap
            );
        }
    }
    else
    {
        p.setColor(normalColorOut);

        if (selected)
        {
            p.setColor(selectedColor);
        }

        painter->setPen(p);
        painter->setBrush(Qt::NoBrush);

        painter->drawPath(cubic);
    }
}

void ConnectionPainter::paint(
    QPainter*                       painter,
    const ConnectionGraphicsObject& cgo
)
{
    drawHoveredOrSelected(painter, cgo);

    drawSketchLine(painter, cgo);

    drawNormalLine(painter, cgo);

#ifdef NODE_DEBUG_DRAWING
    debugDrawing(painter, cgo);
#endif

    // draw end points
    const auto& connectionStyle = QtNodes::StyleCollection::connectionStyle();

    const double pointDiameter = connectionStyle.pointDiameter();

    painter->setPen(connectionStyle.constructionColor());
    painter->setBrush(connectionStyle.constructionColor());
    const double pointRadius = pointDiameter / 2.0;
    painter->drawEllipse(cgo.out(), pointRadius, pointRadius);
    painter->drawEllipse(cgo.in(), pointRadius, pointRadius);
}

}  // namespace QtNodes
