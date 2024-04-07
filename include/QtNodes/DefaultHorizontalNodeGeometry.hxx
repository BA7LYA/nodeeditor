#pragma once

#include <QtGui/QFontMetrics>

#include "QtNodes/AbstractNodeGeometry.hxx"

namespace QtNodes {

class AbstractGraphModel;
class BasicGraphicsScene;

class NODE_EDITOR_PUBLIC DefaultHorizontalNodeGeometry
    : public AbstractNodeGeometry
{
public:
    DefaultHorizontalNodeGeometry(AbstractGraphModel& graphModel);

public:
    QSize size(const NodeId nodeId) const override;

    void recomputeSize(const NodeId nodeId) const override;

    QPointF portPosition(
        const NodeId    nodeId,
        const PortType  portType,
        const PortIndex index
    ) const override;

    QPointF portTextPosition(
        const NodeId    nodeId,
        const PortType  portType,
        const PortIndex PortIndex
    ) const override;
    QPointF captionPosition(const NodeId nodeId) const override;

    QRectF captionRect(const NodeId nodeId) const override;

    QPointF widgetPosition(const NodeId nodeId) const override;

    QRect resizeHandleRect(const NodeId nodeId) const override;

private:
    QRectF portTextRect(
        const NodeId    nodeId,
        const PortType  portType,
        const PortIndex portIndex
    ) const;

    /// Finds max number of ports and multiplies by (a port height + interval)
    unsigned int maxVerticalPortsExtent(const NodeId nodeId) const;

    unsigned int maxPortsTextAdvance(
        const NodeId   nodeId,
        const PortType portType
    ) const;

private:
    // Some variables are mutable because we need to change drawing
    // metrics corresponding to fontMetrics but this doesn't change
    // constness of the Node.

    mutable unsigned int _portSize;
    unsigned int         _portSpasing;
    mutable QFontMetrics _fontMetrics;
    mutable QFontMetrics _boldFontMetrics;
};

}  // namespace QtNodes
