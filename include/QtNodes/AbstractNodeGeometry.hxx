#pragma once

#include <QRectF>
#include <QSize>
#include <QTransform>

#include "QtNodes/Definitions.hxx"
#include "QtNodes/Export.hxx"

namespace QtNodes {

class AbstractGraphModel;

class NODE_EDITOR_PUBLIC AbstractNodeGeometry
{
public:
    AbstractNodeGeometry(AbstractGraphModel&);

    virtual ~AbstractNodeGeometry() {}

    /**
     * The node's size plus some additional margin around it to account for
     * drawing effects (for example shadows) or node's parts outside the size
     * rectangle (for example port points).
     *
     * The default implementation returns QSize + 20 percent of width and
     * heights at each side of the rectangle.
     */
    virtual QRectF boundingRect(const NodeId nodeId) const;

    /// A direct rectangle defining the borders of the node's rectangle.
    virtual QSize size(const NodeId nodeId) const = 0;

    /**
     * The function is triggeren when a nuber of ports is changed or when an
     * embedded widget needs an update.
     */
    virtual void recomputeSize(const NodeId nodeId) const = 0;

    /// Port position in node's coordinate system.
    virtual QPointF portPosition(
        const NodeId    nodeId,
        const PortType  portType,
        const PortIndex index
    ) const
        = 0;

    /// A convenience function using the `portPosition` and a given
    /// transformation.
    virtual QPointF portScenePosition(
        const NodeId      nodeId,
        const PortType    portType,
        const PortIndex   index,
        const QTransform& t
    ) const;

    /// Defines where to draw port label. The point corresponds to a font
    /// baseline.
    virtual QPointF portTextPosition(
        const NodeId    nodeId,
        const PortType  portType,
        const PortIndex portIndex
    ) const
        = 0;

    /**
     * Defines where to start drawing the caption. The point corresponds to a
     * font baseline.
     */
    virtual QPointF captionPosition(const NodeId nodeId) const = 0;

    /// Caption rect is needed for estimating the total node size.
    virtual QRectF captionRect(const NodeId nodeId) const = 0;

    /// Position for an embedded widget. Return any value if you don't embed.
    virtual QPointF widgetPosition(const NodeId nodeId) const = 0;

    virtual PortIndex checkPortHit(
        const NodeId   nodeId,
        const PortType portType,
        const QPointF  nodePoint
    ) const;

    virtual QRect resizeHandleRect(const NodeId nodeId) const = 0;

protected:
    AbstractGraphModel& _graphModel;
};

}  // namespace QtNodes
