///
/// @file DefaultNodePainter.cxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#include "DefaultNodePainter.hxx"

#include <cmath>
#include <QtCore/QMargins>

#include "AbstractGraphModel.hxx"
#include "AbstractNodeGeometry.hxx"
#include "BasicGraphicsScene.hxx"
#include "ConnectionGraphicsObject.hxx"
#include "ConnectionIdUtils.hxx"
#include "NodeGraphicsObject.hxx"
#include "NodeState.hxx"
#include "StyleCollection.hxx"

namespace QtNodes {

void DefaultNodePainter::paint(QPainter* painter, NodeGraphicsObject& ngo) const
{
    // TODO?
    // AbstractNodeGeometry & geometry = ngo.nodeScene()->nodeGeometry();
    // geometry.recomputeSizeIfFontChanged(painter->font());

    drawNodeRect(painter, ngo);
    drawConnectionPoints(painter, ngo);
    drawFilledConnectionPoints(painter, ngo);
    drawNodeCaption(painter, ngo);
    drawEntryLabels(painter, ngo);
    drawResizeRect(painter, ngo);
}

void DefaultNodePainter::drawNodeRect(
    QPainter*           painter,
    NodeGraphicsObject& ngo
) const
{
    AbstractGraphModel&   model    = ngo.graphModel();
    const NodeId          nodeId   = ngo.nodeId();
    AbstractNodeGeometry& geometry = ngo.nodeScene()->nodeGeometry();

    QSize size = geometry.size(nodeId);

    QJsonDocument json
        = QJsonDocument::fromVariant(model.nodeData(nodeId, NodeRole::Style));

    NodeStyle nodeStyle(json.object());

    auto color = ngo.isSelected() ? nodeStyle.SelectedBoundaryColor
                                  : nodeStyle.NormalBoundaryColor;

    if (ngo.nodeState().hovered())
    {
        QPen p(color, nodeStyle.HoveredPenWidth);
        painter->setPen(p);
    }
    else
    {
        QPen p(color, nodeStyle.PenWidth);
        painter->setPen(p);
    }

    QLinearGradient gradient(QPointF(0.0, 0.0), QPointF(2.0, size.height()));

    gradient.setColorAt(0.0, nodeStyle.GradientColor0);
    gradient.setColorAt(0.10, nodeStyle.GradientColor1);
    gradient.setColorAt(0.90, nodeStyle.GradientColor2);
    gradient.setColorAt(1.0, nodeStyle.GradientColor3);

    painter->setBrush(gradient);

    QRectF boundary(0, 0, size.width(), size.height());

    const double radius = 3.0;

    painter->drawRoundedRect(boundary, radius, radius);
}

void DefaultNodePainter::drawConnectionPoints(
    QPainter*           painter,
    NodeGraphicsObject& ngo
) const
{
    AbstractGraphModel&   model    = ngo.graphModel();
    const NodeId          nodeId   = ngo.nodeId();
    AbstractNodeGeometry& geometry = ngo.nodeScene()->nodeGeometry();

    QJsonDocument json
        = QJsonDocument::fromVariant(model.nodeData(nodeId, NodeRole::Style));
    NodeStyle nodeStyle(json.object());

    const auto& connectionStyle = StyleCollection::connectionStyle();

    float diameter        = nodeStyle.ConnectionPointDiameter;
    auto  reducedDiameter = diameter * 0.6;

    for (PortType portType : { PortType::Out, PortType::In })
    {
        const size_t n
            = model
                  .nodeData(
                      nodeId,
                      (portType == PortType::Out) ? NodeRole::OutPortCount
                                                  : NodeRole::InPortCount
                  )
                  .toUInt();

        for (PortIndex portIndex = 0; portIndex < n; ++portIndex)
        {
            QPointF p = geometry.portPosition(nodeId, portType, portIndex);

            const auto& dataType
                = model
                      .portData(nodeId, portType, portIndex, PortRole::DataType)
                      .value<NodeDataType>();

            double r = 1.0;

            const NodeState& state = ngo.nodeState();

            if (const auto* cgo = state.connectionForReaction())
            {
                PortType requiredPort = cgo->connectionState().requiredPort();

                if (requiredPort == portType)
                {
                    ConnectionId possibleConnectionId
                        = makeCompleteConnectionId(
                            cgo->connectionId(),
                            nodeId,
                            portIndex
                        );

                    const bool possible
                        = model.connectionPossible(possibleConnectionId);

                    auto cp
                        = cgo->sceneTransform().map(cgo->endPoint(requiredPort)
                        );
                    cp = ngo.sceneTransform().inverted().map(cp);

                    auto   diff = cp - p;
                    double dist = std::sqrt(QPointF::dotProduct(diff, diff));

                    if (possible)
                    {
                        const double thres = 40.0;
                        r = (dist < thres) ? (2.0 - dist / thres) : 1.0;
                    }
                    else
                    {
                        const double thres = 80.0;
                        r = (dist < thres) ? (dist / thres) : 1.0;
                    }
                }
            }

            if (connectionStyle.useDataDefinedColors())
            {
                painter->setBrush(connectionStyle.normalColor(dataType.id));
            }
            else
            {
                painter->setBrush(nodeStyle.ConnectionPointColor);
            }

            painter->drawEllipse(p, reducedDiameter * r, reducedDiameter * r);
        }
    }

    if (ngo.nodeState().connectionForReaction())
    {
        ngo.nodeState().resetConnectionForReaction();
    }
}

void DefaultNodePainter::drawFilledConnectionPoints(
    QPainter*           painter,
    NodeGraphicsObject& ngo
) const
{
    AbstractGraphModel&   model    = ngo.graphModel();
    const NodeId          nodeId   = ngo.nodeId();
    AbstractNodeGeometry& geometry = ngo.nodeScene()->nodeGeometry();

    QJsonDocument json
        = QJsonDocument::fromVariant(model.nodeData(nodeId, NodeRole::Style));
    NodeStyle nodeStyle(json.object());

    auto diameter = nodeStyle.ConnectionPointDiameter;

    for (PortType portType : { PortType::Out, PortType::In })
    {
        const size_t n
            = model
                  .nodeData(
                      nodeId,
                      (portType == PortType::Out) ? NodeRole::OutPortCount
                                                  : NodeRole::InPortCount
                  )
                  .toUInt();

        for (PortIndex portIndex = 0; portIndex < n; ++portIndex)
        {
            QPointF p = geometry.portPosition(nodeId, portType, portIndex);

            const auto& connected
                = model.connections(nodeId, portType, portIndex);

            if (!connected.empty())
            {
                const auto& dataType = model
                                           .portData(
                                               nodeId,
                                               portType,
                                               portIndex,
                                               PortRole::DataType
                                           )
                                           .value<NodeDataType>();

                const auto& connectionStyle
                    = StyleCollection::connectionStyle();
                if (connectionStyle.useDataDefinedColors())
                {
                    const QColor c = connectionStyle.normalColor(dataType.id);
                    painter->setPen(c);
                    painter->setBrush(c);
                }
                else
                {
                    painter->setPen(nodeStyle.FilledConnectionPointColor);
                    painter->setBrush(nodeStyle.FilledConnectionPointColor);
                }

                painter->drawEllipse(p, diameter * 0.4, diameter * 0.4);
            }
        }
    }
}

void DefaultNodePainter::drawNodeCaption(
    QPainter*           painter,
    NodeGraphicsObject& ngo
) const
{
    AbstractGraphModel&   model    = ngo.graphModel();
    const NodeId          nodeId   = ngo.nodeId();
    AbstractNodeGeometry& geometry = ngo.nodeScene()->nodeGeometry();

    if (!model.nodeData(nodeId, NodeRole::CaptionVisible).toBool())
    {
        return;
    }

    const QString name = model.nodeData(nodeId, NodeRole::Caption).toString();

    QFont f = painter->font();
    f.setBold(true);

    QPointF position = geometry.captionPosition(nodeId);

    QJsonDocument json
        = QJsonDocument::fromVariant(model.nodeData(nodeId, NodeRole::Style));
    NodeStyle nodeStyle(json.object());

    painter->setFont(f);
    painter->setPen(nodeStyle.FontColor);
    painter->drawText(position, name);

    f.setBold(false);
    painter->setFont(f);
}

void DefaultNodePainter::drawEntryLabels(
    QPainter*           painter,
    NodeGraphicsObject& ngo
) const
{
    AbstractGraphModel&   model    = ngo.graphModel();
    const NodeId          nodeId   = ngo.nodeId();
    AbstractNodeGeometry& geometry = ngo.nodeScene()->nodeGeometry();

    QJsonDocument json
        = QJsonDocument::fromVariant(model.nodeData(nodeId, NodeRole::Style));
    NodeStyle nodeStyle(json.object());

    for (PortType portType : { PortType::Out, PortType::In })
    {
        unsigned int n = model.nodeData<unsigned int>(
            nodeId,
            (portType == PortType::Out) ? NodeRole::OutPortCount
                                        : NodeRole::InPortCount
        );

        for (PortIndex portIndex = 0; portIndex < n; ++portIndex)
        {
            const auto& connected
                = model.connections(nodeId, portType, portIndex);

            QPointF p = geometry.portTextPosition(nodeId, portType, portIndex);

            if (connected.empty())
            {
                painter->setPen(nodeStyle.FontColorFaded);
            }
            else
            {
                painter->setPen(nodeStyle.FontColor);
            }

            QString s;

            if (model.portData<bool>(
                    nodeId,
                    portType,
                    portIndex,
                    PortRole::CaptionVisible
                ))
            {
                s = model.portData<QString>(
                    nodeId,
                    portType,
                    portIndex,
                    PortRole::Caption
                );
            }
            else
            {
                auto portData = model.portData(
                    nodeId,
                    portType,
                    portIndex,
                    PortRole::DataType
                );

                s = portData.value<NodeDataType>().name;
            }

            painter->drawText(p, s);
        }
    }
}

void DefaultNodePainter::drawResizeRect(
    QPainter*           painter,
    NodeGraphicsObject& ngo
) const
{
    AbstractGraphModel&   model    = ngo.graphModel();
    const NodeId          nodeId   = ngo.nodeId();
    AbstractNodeGeometry& geometry = ngo.nodeScene()->nodeGeometry();

    if (model.nodeFlags(nodeId) & NodeFlag::Resizable)
    {
        painter->setBrush(Qt::gray);
        painter->drawEllipse(geometry.resizeHandleRect(nodeId));
    }
}

}  // namespace QtNodes