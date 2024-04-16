///
/// @file AbstractNodeGeometry.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef ABSTRACT_NODE_GEOMETRY_HXX_0DC216C46B1A
#define ABSTRACT_NODE_GEOMETRY_HXX_0DC216C46B1A

#include <QRectF>
#include <QSize>
#include <QTransform>

#include "Definitions.hxx"
#include "Export.hxx"

namespace QtNodes {

class AbstractGraphModel;

class NODE_EDITOR_PUBLIC AbstractNodeGeometry
{
public:
    AbstractNodeGeometry(AbstractGraphModel&);

    virtual ~AbstractNodeGeometry() {}

    ///
    /// @brief The node's size plus some additional margin around it to account
    /// for drawing effects (for example shadows) or node's parts outside the
    /// size rectangle (for example port points). The default implementation
    /// returns QSize + 20 percent of width and heights at each side of the
    /// rectangle.
    /// @param nodeId
    /// @return
    ///
    virtual QRectF boundingRect(const NodeId nodeId) const;

    ///
    /// @brief A direct rectangle defining the borders of the node's rectangle.
    ///
    /// @param nodeId
    /// @return QSize
    ///
    virtual QSize size(const NodeId nodeId) const = 0;

    ///
    /// @brief The function is triggeren when a nuber of ports is changed or
    /// when an embedded widget needs an update.
    /// @param nodeId
    ///
    virtual void recomputeSize(const NodeId nodeId) const = 0;

    ///
    /// @brief Port position in node's coordinate system.
    ///
    /// @param nodeId
    /// @param portType
    /// @param index
    /// @return QPointF
    ///
    virtual QPointF portPosition(
        const NodeId    nodeId,
        const PortType  portType,
        const PortIndex index
    ) const
        = 0;

    ///
    /// @brief A convenience function using the `portPosition` and a given
    /// transformation.
    ///
    /// @param nodeId
    /// @param portType
    /// @param index
    /// @param t
    /// @return QPointF
    ///
    virtual QPointF portScenePosition(
        const NodeId      nodeId,
        const PortType    portType,
        const PortIndex   index,
        const QTransform& t
    ) const;

    ///
    /// @brief Defines where to draw port label. The point corresponds to a font
    /// baseline.
    ///
    /// @param nodeId
    /// @param portType
    /// @param portIndex
    /// @return QPointF
    ///
    virtual QPointF portTextPosition(
        const NodeId    nodeId,
        const PortType  portType,
        const PortIndex portIndex
    ) const
        = 0;

    ///
    /// @brief Defines where to start drawing the caption. The point corresponds
    /// to a font baseline.
    ///
    /// @param nodeId
    /// @return QPointF
    ///
    virtual QPointF captionPosition(const NodeId nodeId) const = 0;

    ///
    /// @brief Caption rect is needed for estimating the total node size.
    ///
    /// @param nodeId
    /// @return QRectF
    ///
    virtual QRectF captionRect(const NodeId nodeId) const = 0;

    ///
    /// @brief Position for an embedded widget. Return any value if you don't
    /// embed.
    ///
    /// @param nodeId
    /// @return QPointF
    ///
    virtual QPointF widgetPosition(const NodeId nodeId) const = 0;

    ///
    /// @brief
    /// @param nodeId
    /// @param portType
    /// @param nodePoint
    /// @return
    ///
    virtual PortIndex checkPortHit(
        const NodeId   nodeId,
        const PortType portType,
        const QPointF  nodePoint
    ) const;

    ///
    /// @brief
    /// @param nodeId
    /// @return
    ///
    virtual QRect resizeHandleRect(const NodeId nodeId) const = 0;

protected:
    AbstractGraphModel& _graphModel;
};

}  // namespace QtNodes

#endif  // ABSTRACT_NODE_GEOMETRY_HXX_0DC216C46B1A
